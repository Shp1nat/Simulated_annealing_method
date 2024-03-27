#include <iostream>
#include <vector>
#include <random>
using namespace std;

//Функция для вычисления длины маршрута
int Energy(const vector<int>& L_dist, const vector<vector<int>>& dist) {
    int total_distance = 0; 
    for (int i = 0; i < L_dist.size() - 1; i++) {
        total_distance += dist[L_dist[i]][L_dist[i + 1]]; //складываем все длины между точками маршрута
    }
    return total_distance; //возвращаем длину маршрута
}

//Функция для генерации рандомного нового маршрута
vector<int> Generate_New_L_dist(size_t size) {
    vector<int> new_L_dist; //создаем два вектора на нужный объем
    vector<int> setOfPaths; 
    new_L_dist.reserve(size);
    setOfPaths.reserve(size);
    for (int i = 0; i < size; ++i) {
        setOfPaths.push_back(i); //один из векторов заполняем числами наших контрольный точек (0 до size)
    }
    random_device rd; //random
    mt19937 gen(rd()); //random
    for (int i = (int)size - 1; i >= 0; --i) { //выполняем цикл size раз
        uniform_int_distribution<> distr(0, i); //задаем рандом на число от 0 до i
        int index = distr(gen); //получаем рандомный индекс от 0 до 5 (повториться не может, тк мы удалим его после использования из первого вектора)
        new_L_dist.push_back(*(setOfPaths.begin() + index)); //в новый массив маршрута закидываем наш индекс от 0 до size
        setOfPaths.erase(setOfPaths.begin() + index); //удаляем из первого массива найденный рандомом индекс, чтобы он повторно не попался
    }
    return new_L_dist; //возвращаем новый маршрут
}

//Алгоритм отжига
pair<vector<int>, int> SimulatedAnnealing(const vector<vector<int>>& dist/*массив растояний между вершинами*/, double t_min, double t_max, double alpha, double p_nac) {
    vector<int> L_dist(dist.size()); //массив дистанций пройденных вершин
    L_dist.reserve(dist.size());
    for (int i = 0; i < dist.size(); ++i) { //добавляем путь первого прохождения
        L_dist[i] = i;
    }
    double L_nac = Energy(L_dist, dist); //энергия первого состояния (длина первого прохождения)
    double t_tec = t_max; //текущая температура
    while (t_tec >= t_min) {//цикл выполняется, пока температура нового состаяния не принизит допустимый минимум
        vector<int> new_L_dist = Generate_New_L_dist(L_dist.size()); //генерируем новое состояние (новый путь)
        double L_i = Energy(new_L_dist, dist); //вычисляем энергию нового состояния (длину нового пути)
        double L_d = L_i - L_nac; //находим дельту энергии (разницу путей)
        double p_tec = exp(-L_d / t_tec); //вычисляем вероятность принятия текущего значения, даже если оно не удовлетворяет условиям задачи 
        if (L_d < 0 || p_tec > p_nac) { //проверка на то, является ли путь текущего пути меньше минимума, либо же вычисленная вероятность больше установленного порога
            L_dist = new_L_dist; //присваимваем наш маршрут минимальному
            L_nac = L_i; //присваиваем длину нашего маршрута минимальной
        }
        t_tec *= alpha; //понижение температуры
    }
    return make_pair(L_dist, Energy(L_dist, dist)); //возвращаем ответ в виде пары (маршрут, длина маршрута)
}

vector<vector<int>> makeMatrix() {
    int countOfPoints;
    cout << "Enter the number of points: ";
    cin >> countOfPoints;
    vector<vector<int>> matrixOfWays(countOfPoints, vector<int>(countOfPoints));
    for (int i = 0; i < countOfPoints; ++i) {
        for (int j = i + 1; j < countOfPoints; ++j) {
            int distance;
            cout << "Enter the distance between points " << i + 1 << " and " << j + 1 << ": ";
            cin >> distance;
            matrixOfWays[i][j] = distance;
            matrixOfWays[j][i] = distance;
        }
    }
    cout << "Your matrix is: " << endl;
    for (const auto& el1 : matrixOfWays) {
        for (const auto& el2 : el1) {
            cout << el2 << " ";
        }
        cout << endl;
    }
    return matrixOfWays;
}

int main() {
    //Задание параметров алгоритма
    double t_min = 0.01;
    double t_max = 100.0;
    double alpha = 0.999;
    double p_nac = 0.5;

    auto result = SimulatedAnnealing(makeMatrix(), t_min, t_max, alpha, p_nac);
    cout << "Best path: ";
    for (const auto& el : result.first) {
        cout << el + 1 << " ";
    }
    cout << endl;
    cout << "Best distance: " << result.second;
    return 0;
}