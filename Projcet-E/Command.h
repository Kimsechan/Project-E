#pragma once
void* CommandThread(void* data)
{
	string input;
	while (isRunning)
	{
		cin >> input;
		for (int i = 0; i < input.size(); i++)
		{
			if (input[i] >= 'a' || input[i] <= 'z')
			{
				//�ҹ��ڸ� �빮�ڷ� ������ݴϴ�
				input[i] -= 'A' - 'a';
			};
		};
		switch (input)
		{
		case "QUIT":
		case "END":
		case "EXIT":
			isRunning = false;
			break;
		};
	};
}