#include "neuralNetwork.h"

int main()
{
	loadNueron();
	accessCascade();
	seletCamera();
	accessIpCamera();
	createNumberArray();

	char answer;

	cout << "Do you want to start ALPR?  [ Y/N ]" << endl;
	cin >> answer;

	if (answer == 'y') {
		time_t timer = time(0);
		for (;;) {
			readOriginal();
			processNeuralNetwork();
			if (waitKey(1) >= 0) break;
		}
	}

	else if(answer == 'n') {
		return 0;
	}

	return 0;
}