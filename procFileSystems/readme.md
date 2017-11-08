1) 개요
-1. 사용자로부터 ssu_lsproc는 인풋을 받는다. 
-2. ssu_lsproc는옵션에 따라서 새로운 자식 프로세스를 fork() 해준다.
-3. 부모 프로세스는 자식 프로세스가 끝날 때까지 wait() 한다. 
-4. 부모 프로세스로부터 받은 정보들로 각각의 옵션에 대한 자식 프로세스가 생성되며 옵션에 대한 동작을 실행한다. 
-5. 자식 프로세스가 exit() 되어 종료하게 되면 wait()하고 있던 부모 프로세스가 다시 동작을 한다. 
-6. 그리고 다음 옵션에 대한 과정 1~6을 인풋으로 들어온 모든 옵션을 모두 수행할 때까지 반복한다. 

2)	설계
-uid와 pid를 저장해준다
-사용자로부터 인풋을 받는다. 그때 인자에 13가지 옵션의 사용 유무와 f, t, c, n, m, k 옵션이 뒤에 최대 16개 인자를 받는 것을 고려해 저장해준다.
-16개 이상 받은 인자는 화면에 출력해준다.
-k 옵션이 m옵션와 별도로 또는 먼저 올 수 없는 경우를 고려해준다.
-s옵션의 인자 5가지중 어느 것이 있는지를 표기한다.
-o 옵션의 뒤에 저장 경로를 저장한다.
-옵션이 성공적으로 받아지고 인풋의 저장이 성공적으로 이뤄지면 옵션의 검사를 시작한다.
-우선 o옵션을 사용했다면 먼저 파일의 표준 출력, 에러 출력을 해당 파일로 바꿔준다.
-r옵션을 사용 했다면 사용한 pid와 key를 받는 옵션들의 인풋을 비교해 오름차순 정렬한다.
-f, t, c, n, m, w, e, l, v, s 옵션 순서대로 사용 여부를 검사하면서 사용 했다면 자식 프로세스를 fork()하여 생성 한 뒤 각각의 함수를 호출한다.
-부모 프로세스는 자식 프로세스가 끝날 때까지 wait() 하면서 다시 동작하면 다음 옵션을 위해 위의 단계를 반복한다.
-더 이상 수행할 옵션이 없다면 o옵션의 여부에 따라서 다시 출력을 표준으로 바꾸고 이후 프로그램 종료 문장을 출력한 뒤 끝낸다.