//서버는 아이피를 가지고 시작합니다.
//아이피로 접속을 하는 것이죠
//외부에서 접속할 때에는 퍼블릭 IP가 필요하지만, 서버를 켤 때에는
//내부 공유기 한테 개인 IP로 열 거에요 라고 이야기 할 필요가 있습니다.
//내부 IP를 여기에다가 입력해주시면 됩니다.
#define SERVER_PRIVATE_IP "172.31.4.84"

//컴퓨터에는 동시에 여러개의 프로그램이 작동되고 있습니다.
//만약에 "엘든링"을 하고 있었을 때, 네트워크를 사용하고 있죠.
//몬스터 헌터 를 같이 켜뒀습니다. 아이피만 가지고 대화를 시도하면요. 들어온 메세지가 엘든링 것인지, 몬스터 헌터 것인지 전혀 알 수가 없어요.
//"Port"라고 하는 것이 누구 메세지인지 구분 할 수 있게 해줍니다.
//"이 포트는 이 게임이고 저 포트는 저 게임이야." 라고 하는 느낌
//49152 ~ 65535 가 자유롭게 사용할 수 있는 "동적 포트"니까 이 사이에 있는 값으로 조정해줄게요
#define SERVER_PORT 55555

//서버에서는 메세지를 보낼 겁니다.
//메세지 무한정 보낼 수는 없습니다. 네트워크 계층에는 물리계층이 있는데 물리적인 한계가 존재할 수 밖에 없죠
//저희가 그렇게 까지 큰 메세지를 많이 보내진 않을거니까 가볍게 최대 버퍼사이즈를 조정해주도록 합시다.
#define MAX_BUFFER_SIZE 1024

//최대값을 정해야하는 다른 수가 있을 거에요
//일단 테스트 삼아 최대 인원수를 100명으로 테스트 해보겠습니다.
#define MAX_USER_NUMBER 100

#include <iostream>

//지금 쓰고있는 것은 Window 가 아닌 Linux Header들 입니다.
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>

#include <sys/timeb.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

//Poll 이라고 하는 것은 상대방이 반을을 해줬을때, 인식해서 돌려주는 개념이에요.
//누군가 저한테 메세지를 전해줬을 때 서버 내용이 돌아가도록
//FD 는 무엇인가. FD = File Descripter 라고하는 파일을 설명해주는 녀석입니다.
//소켓을 형상화 한 것인데 Linux 는 소켓이라고 하는 것도 일종의 파일 형태로 관리합니다.
//파일 번호를 저장할 수 있는 공간이 될 겁니다. 소켓에 해당되는 파일 정보
struct pollfd pollFDArray[MAX_USER_NUMBER];

int StartServer(int currentFD);

int main()
{
	//듣기만 하는 fd가 필요해요	
	//사실 이상한 애가 와서 이야기를 하면 안들어야겠죠
	//컴퓨터는 처음 보는 IP가 와서 이야기 하면 "전화 잘못거셨어요." 라고 하면서 끊어버려요
	//ListenFD 라고 하는 애는 잘 들어줄 겁니다.
	//아하 이분이 지금 접속을 하고 싶으신가 봅니다.
	//다른 FD 한테 새로운 접속을 알려주는 역할로 둘 거에요 (입구 역할을 하는 겁니다.)
	//0번째 유저를 리슨소켓으로 사용할 겁니다.
	struct poolfd& ListenFD = pollFDArray[0];


					//IPv4(4바이트짜리 IP)
	ListenFD.fd = socket(AF_INET, SOCK_STREAM, 0);
	ListenFD.events = POLLIN;
	ListenFD.revents = 0;

	//			0은 리슨소켓이니까
	for (int i = 1; i < MAX_USER_NUMBER, i++)
	{
		//File Descripter가 등록되지 않은 PollFD는 연결이 안된 것입니다.
		//해당 되는 소켓이 없다는 뜻 입니다.
		pollFDArray[i].fd = -1;
	};

	//여기서 FD 는 준비가 되었고 서버를 돌려봅시다.
	//Listen Socket에 정보를 전달해주면서 서버를 시작할 거에요
	StartServer(ListenFD.fd);

	while (true)
	{
		//poll에 대해서 말씀을 드릴 때 누군가 저한테 메세지를 전달했을 때 실행
		//0번까지도 폴에 넣어서 리슨 소켓에 대답이 있을 때에도 들어갈 수 있게 위에서 설정해줬어요.
		int result = poll(pollFDArray, MAX_USER_NUMBER, -1);

		//누가 부른던데요? 0이면 아무도 대답안했다 15라고 한다면, 15명이 부른다!
		if (result > 0)
		{
			//리슨 소켓에 반응 확인
			//누군가 접속을 시도하고 있습니다.
			if (ListenFD.revents == POLLIN)
			{
				cout << "누군가의 반응 식별" << endl;
			};
		};
	};

	//리슨 소켓 닫고
	close(ListenFD.fd);

	for (int i = 0; i < MAX_USER_NUMBER; i++)
	{
		//엇 누구 있어? 닫아.
		if (pollFDArray[i].fd != -1) close(pollFDArray[i].fd);
	}
	return -4;
}

int StartServer(int currentFD)
{
	//서버를 시작하려고 하는데,
	//Listen Socket이 막혀있어요.
	if (currentFD == -1)
	{
		//Socket 때문에 Error가 났다고 알려줍니다.
		// perror = print Error
		perror("socket()");
		//혹시 모르니깐, Socket 꺼주기.
		close(currentFD);
		return -1;
	};

	//Socket에다가 INET 이라는 정보를 넣어줬는데요.
	//그래.. 4바이트 짜리 IP 인 것은 알겠는데, 그래서 IP가 뭔데?
	//그래서 저희는 서버의 IP를 리슨 소켓에다가 등록을 해주어야 합니다
	sockaddr_in address;

	//혹시 모르니까 완전 초기화를 해놓고 정보를 넣을게요
	memset(&address, 0, sizeof(address));

	//소켓이랑 아이피 종류를 맞춰주도록 할게요
	address.sin_family = AF_INET;
	//실제 서버 IP를 여기에서 넣어주도록 합시다.
	address.sin_addr.s_addr = inet_addr(SERVER_PRIVATE_IP);
	//포트까지 적어주어야 했겠죠
	address.sin_port = hton(SERVER_PORT);
	
	//주소를 여기에요 로 끝난다면 서버가 실행이 안 될 거에요. 저장만 한 것이죠
	//사용해서 서버를 만들어야 할 거니까
	//지금 설정한 주소를 소켓에다가 "묶어" 줄 거에요					-1 = 실패
	if (bind(currentFD, (struct sockaddr*)&address, sizeof(address)) == -1)
	{
		perror("bind()");
		close(currentFD);
		return -1;
	};

	//실제로 리슨 소켓을 작동을 시켜봅시다.
	//그럼 진짜로 서버가 돌아가는 거겠죠
	if (listen(currentFD, 8) == -1)
	{
		perror("listen()");
		close(currentFD);
		return -1;
	};
	
	//당신은 모든 시련을 훌륭하게 이겨내셨습니다.
	return 1;
}