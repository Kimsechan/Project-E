#pragma once

enum class CommandType
{
	Error,
	Quit,

};

//���� ������� ��ɾ� ����
CommandType currentType;
//���� ��ɾ �� ĭ�̳� �о��°�?
int currentIndex;

void CalculatedCommand(string word)
{
	//���� �ƹ��͵� �� �޾Ҿ��
	//�� ���� �������� ���� ���� ��ɾ �� ������ ������ �ð��̶�� ���Դϴ�.
	if (currentIndex == 0)
	{
		if (word == "QUIT" || word == "EXIT" || word == "CLOSE" || word == "END")
		{
			currentType = CommandType::Quit;
		};
	};

	++currentIndex;
}

void ExcuteCommand()
{
	switch (currentType)
	{
	case CommandType::Quit:
		isRunning = false;
		break;
	};
}

void* CommandThread(void* data)
{
	string input;
	while (isRunning)
	{
		//�������� �Է��� ����
		cin >> input;

		//��ɾ �ʱ�ȭ�ϰ� ���Կ�
		currentType;
		//���� ���� ���� �����ϱ� 0��°
		currentIndex = 0;

		for (int i = 0; i < input.size(); i++)
		{
			if (input[i] >= 'a' || input[i] <= 'z')
			{
				//�ҹ��ڸ� �빮�ڷ� ������ݴϴ�
				input[i] -= 'A' - 'a';
			};
		};

		string currentWord;
		for (int i = 0; i <= input.size(); i++)
		{
			//���ڰ� �����ų�, �����̽��� ������ �� �ڸ����� ���� �о��
			if (i == input.size() || input[i] == ' ')
			{
				CalculatedCommand(currentWord);
				currentWord.clear();
			}
			else //�����̽��� �ƴ϶�� �ܾ�ٰ� ���ڸ� �߰� �� �̴ϴ�
			{
				currentWord += input[i];
			};
		};
		//���ڸ� ��� �ľ������� ������ Ŀ�ǵ带 �����մϴ�
		ExcuteCommand();
	};

	return nullptr;
}