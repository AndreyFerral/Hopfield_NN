#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

class HopfieldNetwork {
private:
	int count_n; // размер матрицы весов
	int count_row; // кол-во образов для запоминания
	vector<vector<double>> weights; // веса
	vector<vector<int>> target; // образа для распознавания
public:
	HopfieldNetwork(vector<vector<int>> target) {
		this->target = target;
		// Получаем размер матрицы весов
		count_row = target.size();
		count_n = target[0].size();
		// Заполняем матрицу весов count_n*count_n значением 0
		weights.resize(count_n, vector<double>(count_n, 0));
		// Запускаем обучение нейронной сети
		train(target);
	}

	void train(vector<vector<int>> target) {
		// Перебираем строки в матрице target
		for (int row = 0; row < count_row; row++) {
			// Перебираем значения в строке target
			for (int i = 0; i < count_n; i++) {
				// Снова перебираем значения в строке target
				for (int j = 0; j < count_n; j++) {
					weights[i][j] = weights[i][j] + target[row][i] * target[row][j];
					// На последнем выполнении
					if (row + 1 == count_row) {
						// Обнуляем диагональную матрицу
						if (i == j) { weights[i][j] = 0; }
						// Умножаем значение веса на 1/count_n
						else { weights[i][j] = weights[i][j] * 1 / count_n; }
					}
				}
			}
		}
	}

	void recognize(vector<int> inputs) {
		vector<double> y(count_n, 0);
		// Перебираем значения в inputs
		for (int i = 0; i < count_n; i++) {
			// Перебираем значения в weights
			for (int j = 0; j < count_n; j++) {
				y[i] = y[i] + weights[i][j] * inputs[j];
			}
		}

		// Если y[i]>=0, то 1, иначе -1
		vector<double> inputs_new(count_n, 0);
		for (int i = 0; i < count_n; i++) {
			inputs_new[i] = sign(y[i]);
		}

		// Выводим в консоль количество различий
		vector<double> inputs_d(inputs.begin(), inputs.end());
		output_diff(inputs_d, inputs_new);

		// Сверяем полученный образ с запомненными
		for (int i = 0; i < count_row; i++) {
			bool is_correct = true;
			for (int j = 0; j < count_n; j++) {
				if (target[i][j] != inputs_new[j]) {
					is_correct = false;
				}
			}
			if (is_correct) { 
				vector<double> target_d(target[i].begin(), target[i].end());
				cout << setw(25) << "Найдено совпадение с: "; output_vector(target_d);
				return;
			}
		}
		cout << "Совпадней не было найдено.";
	}

	void output_diff(vector<double> inputs, vector<double> inputs_new) {
		int count_diff = 0;
		for (int i = 0; i < count_n; i++) {
			if (inputs[i] != inputs_new[i]) { 
				count_diff++; 
			}
		}
		cout << setw(25) << "Образ для распознавания: "; output_vector(inputs);
		cout << setw(25) << "Полученный образ: "; output_vector(inputs_new);
		cout << setw(25) << "Количество различий: " << count_diff << '\n';
	}

	double sign(double x) {
		return (x < 0) ? -1 : 1;
	}

	void output_weights() {
		for (int i = 0; i < count_n; i++) {
			for (int j = 0; j < count_n; j++) {
				cout << weights[i][j] << ' ';
			}
			cout << '\n';
		}
	}

	void output_vector(vector<double> vector) {
		for (int i = 0; i < count_n; i++) {
			cout << vector[i] << ' ';
		}
		cout << '\n';
	}
};

int main()
{
	setlocale(LC_ALL, "Russian");
	// Задаем образы для тренировки нейронной сети
	vector<vector<int>> target = {
		{1, 1, 1, 1, -1, 1, 1, -1, 1},
		{1, 1, 1, 1, -1, -1, 1, -1, -1},
		{1, -1, -1, 1, -1, -1, 1, 1, 1}
	};
	HopfieldNetwork hopfield(target);

	// Задаем образ для распознавания
	vector<int> inputs = { 1, -1, -1, 1, -1, -1, 1, 1, -1 };
	hopfield.recognize(inputs);
}