# LearnUs 제출 상태별 복원본

2026-10-25에 본인의 제출 압축파일에서 각각 복원한 xv6 RISC-V 프로젝트입니다. 기존 MIT LICENSE와 수업 제공 구조를 유지합니다. 학번·이름 출력은 0 / JOON0512로 바꾸고, 개인 제출용 tar 스크립트는 제외했습니다.

| 폴더 | 과제 주제 |
|---|---|
| assignment-0 | 설치·기본 실행 |
| assignment-1 | ysh 셸 |
| assignment-2 | 프로세스 조회 syscall / ps |
| assignment-4 | 메모리 할당·free list |
| assignment-5 | 페이징 |
| assignment-6 | 사용자 스레드 |

각 폴더에서 `make kernel/kernel fs.img` 후 `make qemu CPUS=1`로 실행합니다. RISC-V GNU 툴체인과 QEMU가 필요합니다. 여섯 버전 모두 1 CPU QEMU에서 빌드·부팅·`usertests -q`를 통과했습니다. 이 기본 회귀 검사가 각 과제의 요구사항 전체나 다중 CPU 동시성을 검증하는 것은 아닙니다.

스케줄링 과제의 LearnUs 제출 첨부파일은 확인되지 않았습니다. 저장소 루트의 stride 버전은 별도로 로컬에 남아 있던 실습 보관본입니다. 제출본들은 누적 변경이 아니므로 폴더 사이의 파일을 섞어서 빌드하지 마세요.

추가 실행: `ysh`의 echo, `ps`, 30회 `malloctest`, 7개 `threadtests`(반복 실행 포함)가 모두 완료됐습니다. 스레드 테스트는 모든 항목 pass를 출력했습니다.
