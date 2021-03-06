//통합! 이라고 하는 것이죠!
//Struct랑 비슷하게 쓰실 수 있습니다!
//차이점이 무엇이냐? 하나의 메모리를 여러가지 자료형이 공유해요!

//[0][0][0][10]
//          10  int
//          \n  char[3]
union ConvertionBase
{
	unsigned int uInteger;
	int integer;
	float floating;
	char character[4];
	short shortInteger[2];
	unsigned short uShortInteger[2];
};
ConvertionBase byteConvertor;

//항상 클라이언트의 enum값과 일치시켜주세요!
enum class MessageType
{
	EndOfLine, //문자열의 끝을 판단할 때 \0이라고 하는 걸 파악했습니다! 메시지가 뒤에 더 없다 라고 하는 걸 EndOfLine으로 표시!
	SignUp,
	LogIn,
	LogOut,
	Chat,
	Input,

	Unknown,  //서버 전용 모르겠다 선언!
	Length
	//제가 가진 메시지 타입의 개수보다 더 많은 내용이 들어오면 무시!
};

enum class InputType
{
	Button0,
	Button1,
	Button2,
};

string SaveAsHashCode(string input)
{
	return sha256(input);
}

class MessageInfo
{
public:
	MessageType type;
	int length;
	int userIndex;
};

class MessageInfo_Login : public MessageInfo
{
public:
	string name;
	string password;
	MessageInfo_Login(char* message, int targetUser)
	{
		type = MessageType::LogIn;

		userIndex = targetUser;
		//메시지의 4번째부터 내용을 넣어주도록 합시다!
		name = &(message[4]);
		int currentIndex = 4;
		//0이 될 때까지 이동하기
		while (message[currentIndex] != 0) ++currentIndex;

		//0이 되는 다음 칸 부터 password로 입력
		++currentIndex;
		password = &(message[currentIndex]);

		//비밀번호를 해시코드로 변환해서 저장합시다
		password = SaveAsHashCode(password);
	}
};

class MessageInfo_SignUp : public MessageInfo
{
public:
	string name;
	string password;
	string nicname;
	MessageInfo_SignUp(char* message, int targetUser)
	{
		type = MessageType::SignUp;

		userIndex = targetUser;
		//메시지의 4번째부터 내용을 넣어주도록 합시다!
		name = &(message[4]);
		int currentIndex = 4;
		//0이 될 때까지 이동하기
		while (message[currentIndex] != 0) ++currentIndex;

		//0이 되는 다음 칸 부터 password로 입력
		++currentIndex;
		password = &(message[currentIndex]);

		//비밀번호를 해시코드로 변환해서 저장합시다
		password = SaveAsHashCode(password);

		//0이 되는 다음 칸 부터 password로 입력
		while (message[currentIndex] != 0) ++currentIndex;
		//0이 되는 다음 칸 부터 Nickname으로 입력
		++currentIndex;
		nicname = &(message[currentIndex]);
	}
};

class MessageInfo_Chat : public MessageInfo
{
public:
	string value;

	MessageInfo_Chat(char* message, int targetUser)
	{
		type = MessageType::Chat;

		userIndex = targetUser;
		//메시지의 4번째부터 내용을 넣어주도록 합시다!
		value = &(message[4]);
	}
};

class MessageInfo_Input : public MessageInfo
{
public:
	InputType currentType;

	MessageInfo_Input(InputType wantType, int targetUser)
	{
		type = MessageType::Input;

		currentType = wantType;
		userIndex = targetUser;
	}
};