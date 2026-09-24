# AGENTS.md

Guidance for AI coding agents working in this repository.

## Project overview

zOS is a small bare-metal firmware for the **STM32F100xB** (Cortex-M3, STM32VLDiscovery-class
board: 128 KB flash, 8 KB RAM). It is not a preemptive RTOS: `main()` runs a cooperative,
round-robin task list driven by a 1 ms SysTick, plus a simple command shell on USART1.

- Language: C (plus the ST startup assembly file from the StdPeriph library).
- Toolchain: **IAR Embedded Workbench for ARM** (project saved with 6.10). There is no
  Makefile, CMake, or GCC build.
- Library: **STM32F10x Standard Peripheral Library V3.5.0**, *not* vendored in this repo.

## Repository layout

| Path | Purpose |
| --- | --- |
| `src/main.c` | Init sequence and the super-loop (`SysTick_1ms()` → `TaskRun()`). |
| `src/roundrobintask.[ch]` | Cooperative scheduler: `TTASK` linked list, `TaskAdd`, `TaskRun`, `TaskMonitor`. |
| `src/systick.[ch]` | 1 ms SysTick tick counter. |
| `src/cmd.[ch]` | Serial command shell (`z> ` prompt) with a `TCMD_TABLE` of commands. |
| `src/usart.[ch]` | USART1/USART2 drivers with interrupt-driven RX queues; `zprint()` writes to USART1. |
| `src/led.[ch]` | LED blink modes on PC8/PC9. |
| `src/button.[ch]` | Debounced button on PA0 with toggle/latch helpers. |
| `src/queue.[ch]`, `src/stack.[ch]` | Fixed-size ring buffer and stack utilities. |
| `src/util.[ch]` | Minimal string helpers (`zstrcmp`, `zstrcmpnc`, `zstrlen`, `zstrcpy`). |
| `src/header.h` | Common include: pulls in `stm32f10x.h`, defines `uint8/16/32`, `int8/16/32`, `bitN` masks, `KickDog()`. |
| `src/stm32f10x_conf.h`, `src/stm32f10x_it.[ch]` | StdPeriph config and exception handlers. |
| `src/system_stm32f10x.c` | Local copy of the CMSIS system file — **not** built; the project compiles the library's copy. |
| `stm32f100xb.icf` | IAR linker script (ROM `0x08000000–0x0801FFFF`, RAM `0x20000000–0x20001FFF`, 1 KB stack, 512 B heap). |
| `zOS.eww`, `zOS.ewp`, `zOS.ewd`, `zOS.dep`, `settings/` | IAR workspace/project/debugger files. |

## Building

The build only works inside IAR EWARM on Windows, and expects the StdPeriph library two
directories above the project:

```
<root>/STM32F10x_StdPeriph_Lib_V3.5.0/
<root>/<something>/zOS/        <- this repo
```

Open `zOS.eww` and build the `Debug` configuration. The project defines `USE_STDPERIPH_DRIVER`
and adds `src/` plus the CMSIS include paths.

Agents in a Linux sandbox generally **cannot build or flash** this project. When you change code:

- Keep changes small and review them carefully by reading the code, because nothing will compile them for you.
- If you add or remove a `.c`/`.h` file, also add or remove the matching `<file>` entry in
  `zOS.ewp` (paths use `$PROJ_DIR$\src\...` with Windows backslashes), or the IAR build will not pick it up.
- Don't hand-edit `zOS.dep`, `zOS.ewd`, or `settings/*`; IAR regenerates them.
- Don't edit the `###ICF###` section of `stm32f100xb.icf` by hand unless you mean to change the memory map.
- Say in your summary that the change was not compiled or tested on hardware.

## Architecture and conventions

### Adding a periodic task

Tasks are statically allocated `TTASK` structs registered with `TaskAdd`. `servicetime` is in
scheduler ticks (1 tick = 1 ms), so `10` means every 10 ms:

```c
static TTASK mytask;

static void MyTask_10ms(void *param) { /* must return quickly */ }

void My_Init(void)
{
  TaskAdd(&mytask, "MY", 10, MyTask_10ms, 0);
}
```

Then call `My_Init()` from `main()` after `SysTick_Init()`. Tasks are cooperative: never block or
busy-wait inside a task. Tasks cannot be removed once added. The `CMD` shell's `task` command
lists every registered task.

### Adding a shell command

Add a handler with the signature `uint8 Cmd_X(uint8 *cmd, uint8 len, TPRINT print)` to
`src/cmd.c` and an entry to `cmdtable[]`. Index 0 is reserved, and the `{"",0}` entry must stay
last as the terminator. Commands match on a case-insensitive prefix of the command name.

### Code style

Match the existing code:

- Two-space indentation, braces on their own line, no space after `if`/`while`/`for`.
- Use the project's `uint8`/`uint16`/`uint32` typedefs from `header.h` (not `uint8_t` or `u8` in new code).
- Include `header.h` first in every `.c` file.
- Every file starts with the `/*--- ** Program / Project / CPU / Date / Modify ... ---*/` header block.
- Group code in a file under `//Application Layer`, `//Middle Layer`, and `//Physical Driver` sections.
- Public functions use a `Module_Name` / `ModuleAPI_Name` prefix; file-local helpers are `static`
  and declared at the top of the file. Types are `T`-prefixed uppercase (`TTASK`, `TCMD`).
- No dynamic allocation (`malloc`) and keep RAM use low: there is only 8 KB total.
- Use `zprint()` for debug output on USART1; `printf`/`sprintf` are not wired up.
- Header guards use `_NAME_H`.

## Git

- Commit messages are short, lowercase, and imperative (e.g. `add button latch,remove define in cmd.`).
- Don't commit IAR build output (`Debug/`, `*.o`, `*.out`, `*.map`, etc.).
