# xv6 운영체제 실습

연세대학교 운영체제 수업에서 작업한 xv6 RISC-V 소스를 본인의 로컬 보관 압축파일에서 복원했습니다. MIT xv6와 수업 제공 코드를 기반으로 한 실습이며, 전체 운영체제를 처음부터 작성한 프로젝트는 아닙니다. 원래 설명은 [README](README), 저작권 및 이용 조건은 [LICENSE](LICENSE)에 보존했습니다.

복원된 버전에는 ticket/stride 기반 스케줄러, 실행 통계, `sdbg on/off`, `whoami`가 있습니다. 로컬에 남아 있던 하나의 실습 버전이며, 학기 전체 과제와 제출 이력을 모두 복원했다는 의미는 아닙니다. 공개 복사본의 `user/sid.h`는 학번 대신 0, 이름 대신 GitHub ID를 사용합니다.

## 실행

RISC-V GNU 툴체인(`riscv64-unknown-elf-*`)과 `qemu-system-riscv64`, C 컴파일러, make가 필요합니다.

```sh
make
make fs.img
make qemu CPUS=1
```

xv6 셸에서 `whoami`, `sdbg on`, `sdbg off`, `usertests`를 실행할 수 있습니다. QEMU 종료는 Ctrl-A를 누른 뒤 X입니다. 이 복원본은 **CPU 1개** 설정으로 검증했습니다. 다중 CPU에서 스케줄러의 정확성·경합·공정성을 검증하지 않았습니다.

## 검증 기록

2026-10-24, macOS의 RISC-V 툴체인과 QEMU에서 커널·파일시스템 빌드 및 부팅을 확인했습니다. 일반 `usertests` 항목과 `bigdir`, `manywrites`를 통과했습니다. 전체 실행의 시간 제한 이후 남은 `badwrite`, `execout`, `diskfull`, `outofinodes`를 각각 재실행하여 모두 통과했습니다. 하나의 중단 없는 전체 실행을 통과한 기록은 아닙니다.

`tar.sh`는 원래 수업 제출용 보조 스크립트입니다. 일반 빌드·실행에는 필요하지 않습니다. 스케줄러의 정책 자체가 올바른지는 별도의 공정성·장기 실행 평가가 필요합니다.

## 추가 복원 (2026-10-25)

LearnUs에서 실제 제출 첨부파일 6개를 확보해 [submissions](submissions)에 과제별로 추가했습니다. 셸·시스템콜·free list·페이징·스레드 실습을 포함하며 모두 QEMU 1 CPU의 `usertests -q`를 통과했습니다. 루트의 stride 버전은 LearnUs에서 내려받은 스케줄링 제출본으로 확인된 자료가 아닙니다.
