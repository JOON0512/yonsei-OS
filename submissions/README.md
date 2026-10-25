# xv6 assignments

운영체제 수업의 과제별 xv6 버전.

| 폴더 | 내용 |
| --- | --- |
| [assignment-0](assignment-0) | 기본 실행 |
| [assignment-1](assignment-1) | `ysh` 셸 |
| [assignment-2](assignment-2) | 프로세스 조회 syscall과 `ps` |
| [assignment-4](assignment-4) | 메모리 할당과 free list |
| [assignment-5](assignment-5) | 페이징 |
| [assignment-6](assignment-6) | 사용자 스레드 |

각 폴더에서 RISC-V GNU 툴체인과 QEMU로 실행합니다. 폴더 사이의 파일을 섞지 않습니다.

```sh
make kernel/kernel fs.img
make qemu CPUS=1
```

기본 검사는 xv6 셸의 `usertests -q`, 스레드 검사는 assignment-6의 `threadtests`입니다. 여섯 버전의 기본 검사와 일곱 스레드 테스트를 단일 CPU에서 확인했습니다.

루트의 stride 스케줄러는 이 제출본들과 별도로 작업한 버전입니다. MIT xv6와 수업 제공 코드를 기반으로 합니다.
