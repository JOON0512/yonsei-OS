# xv6 labs

연세대 운영체제 수업의 [xv6-riscv](https://github.com/mit-pdos/xv6-riscv) 실습.

루트에는 ticket/stride 스케줄러와 실행 통계가 있고, [submissions](submissions)에는 셸·시스템콜·메모리·스레드 과제가 각각 들어 있습니다. 각 폴더는 독립된 xv6 버전입니다.

## Run

RISC-V GNU 툴체인과 `qemu-system-riscv64`가 필요합니다. 실행할 버전의 폴더에서:

```sh
make kernel/kernel fs.img
make qemu CPUS=1
```

QEMU 종료: **Ctrl-A → X**.

루트 버전의 추가 명령은 `whoami`, `sdbg on`, `sdbg off`입니다. 기본 회귀 검사는 xv6 셸에서 `usertests`로 실행합니다.

테스트는 단일 CPU에서 진행했습니다. 다중 CPU의 스케줄러 동기화와 공정성은 별도 확인이 필요합니다.

MIT xv6와 수업 제공 코드를 기반으로 합니다. [원본 README](README) · [License](LICENSE)
