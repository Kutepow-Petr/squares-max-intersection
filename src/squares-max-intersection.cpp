#include <locale.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

void PrintCrd(ofstream& file, float** Arr, short pnt1, short pnt2, short pnt3, short pnt4) {

    file << "(" << *(*(Arr + pnt1) + 0) << ";" << *(*(Arr + pnt1) + 1) << "), ("
        << *(*(Arr + pnt2) + 0) << ";" << *(*(Arr + pnt2) + 1) << "), ("
        << *(*(Arr + pnt3) + 0) << ";" << *(*(Arr + pnt3) + 1) << "), ("
        << *(*(Arr + pnt4) + 0) << ";" << *(*(Arr + pnt4) + 1) << ")";
}

void PrintRes(float** ArrA, float*** ArrS, short count_s, short count_a, short N, ofstream& result) {

    result << "Задание:\n"
        << "Дано N точек на плоскости. Найти все пары квадратов с вершинами в этих точках,\n"
        << "имеющих наибольшую площадь пересечения. Вывод вершин осуществить против часовой\n"
        << "стрелки. Стороны квадратов расположены параллельно.\n\n"
        << "ОБРАБОТКА ДАННЫХ:\n";
    if (N == 0) { result << "Кол-во точек = 0."; return; }
    else result << "На обработку взято " << N << " точек." << "\nПостроено " << count_s << " квадратов.\n\n";
    result << "РЕЗУЛЬТАТ:\n";

    float max = 0;
    for (short i = 0; i < count_a; i++) {
        if (max < *(*(ArrA + i) + 0)) max = *(*(ArrA + i) + 0);
    }
    if (max > 0) {
        result << "Наибольшая площадь пересечения, равная = " << max << ", у\n";
        for (short i = 0; i < count_a; i++) {
            if (*(*(ArrA + i) + 0) == max) {
                short sqr1 = *(*(ArrA + i) + 1);
                short sqr2 = *(*(ArrA + i) + 2);
                result << "квадрата с вершинами в точках ";
                PrintCrd(result, *(ArrS + sqr1), 4, 5, 6, 7);
                result << "\nи квадрата с вершинами ";
                PrintCrd(result, *(ArrS + sqr2), 4, 5, 6, 7);
                result << "\n\n";
            }
        }
    }
    else result << "Никакие квадраты не пересекаются.\n";
}

bool CalcSize(ifstream& points, ofstream& result, short& N0) {

    points >> N0;
    if (points.eof()) { result << "Файл пуст."; return -1; }
    else if (N0 < 0) { result << "Кол-во точек не может быть отрицательным."; return -1; }
    else if (N0 == 0) { result << "Кол-во точек = 0."; return -1; }
    else if (N0 > 100) { result << "Слишком большое кол-во точек."; return -1; }
    return 0;
}

void GetPoints(ifstream& points, ofstream& protocol, float** ArrP, short N0, short& N) {

    points >> noskipws;
    char sym; short crd = -1;
    while (!points.eof() && N < N0) {
        points >> sym;
        switch (sym) {
        case ' ': { continue; break; }
        case '\t': { continue; break; }
        case '\n': {
            switch (crd) {
            case 0: { 
                protocol << "Пустая строка.\n";
                break;
            }
            case 1: {
                protocol << "Не удалось взять точку (" << *(*(ArrP + N) + 0) << ";?). Недостаточно данных.\n";
                break;
            }
            }
            crd = 0;
            continue; break;
        }
        default: {
            switch (crd) {
            case 0: {
                points.seekg(-1, ios::cur);
                points >> *(*(ArrP + N) + crd);
                crd++;
                continue; break;
            }
            case 1: {
                points.seekg(-1, ios::cur);
                points >> *(*(ArrP + N) + crd);
                bool flag_same = 0;
                for (short same = 0; same < N; same++) {
                    if (*(*(ArrP + same) + 0) == *(*(ArrP + N) + 0) && *(*(ArrP + same) + 1) == *(*(ArrP + N) + 1)) flag_same = 1;
                }
                if (flag_same == 0) {
                    protocol << "Получена " << N + 1 << "-я точка с координатами (" << *(*(ArrP + N) + 0) << ";" << *(*(ArrP + N) + 1) << ")\n";
                    crd++;
                    N++;
                }
                else protocol << "Точка (" << *(*(ArrP + N) + 0) << ";" << *(*(ArrP + N) + 1) << ") уже записана.\n"; crd = 2;
                continue; break;

            }
            default: {
                while (sym != '\n' && !points.eof()) points >> sym;
                points.seekg(-1, ios::cur);
                continue; break;
            }
            }
        }
        }
    }
}

void Transform(float (&pnt)[2], float** ArrP, short nump, float angle) {

    pnt[0] = *(*(ArrP + nump) + 0) * cos(angle) - *(*(ArrP + nump) + 1) * sin(angle);
    pnt[1] = *(*(ArrP + nump) + 0) * sin(angle) + *(*(ArrP + nump) + 1) * cos(angle);
}

bool AddSquare(float*** ArrS, float** ArrP, short& count, short pnt1, short pnt2, short pnt3, short pnt4) {

    float angle = -atan((*(*(ArrP + pnt2) + 0) - *(*(ArrP + pnt1) + 0)) / (*(*(ArrP + pnt1) + 1) - *(*(ArrP + pnt2) + 1)));
    float top[4][2];
    Transform(top[0], ArrP, pnt1, angle);
    Transform(top[1], ArrP, pnt2, angle);
    Transform(top[2], ArrP, pnt3, angle);
    Transform(top[3], ArrP, pnt4, angle);

    short same = 0;
    for (short sqr = 0; sqr < count; sqr++) {
        for (short pnt = 0; pnt < 4; pnt++) {
            if (*(*(*(ArrS + sqr) + pnt) + 0) == top[pnt][0] && *(*(*(ArrS + sqr) + pnt) + 1) == top[pnt][1]) same++;
        }
    }

    if (same >= 4) return -1;
    else {
        for (short crd = 0; crd < 2; crd++) {
            *(*(*(ArrS + count) + 0) + crd) = top[0][crd];
            *(*(*(ArrS + count) + 1) + crd) = top[1][crd];
            *(*(*(ArrS + count) + 2) + crd) = top[2][crd];
            *(*(*(ArrS + count) + 3) + crd) = top[3][crd];
            *(*(*(ArrS + count) + 4) + crd) = *(*(ArrP + pnt1) + crd);
            *(*(*(ArrS + count) + 5) + crd) = *(*(ArrP + pnt2) + crd);
            *(*(*(ArrS + count) + 6) + crd) = *(*(ArrP + pnt3) + crd);
            *(*(*(ArrS + count) + 7) + crd) = *(*(ArrP + pnt4) + crd);
        }
        count++;
        return 0;
    }
}

void GetSquare(ofstream& protocol, float** ArrP, float*** ArrS, short N, short& count_s) {

    for (short top1 = 0; top1 < N; top1++) {
        for (short top2 = 0; top2 < N; top2++) {
            if (top2 == top1) continue;
            if (*(*(ArrP + top1) + 0) <= *(*(ArrP + top2) + 0)&& *(*(ArrP + top1) + 1) > *(*(ArrP + top2) + 1)) {
                float deltaX = *(*(ArrP + top2) + 0) - *(*(ArrP + top1) + 0);
                float deltaY = *(*(ArrP + top1) + 1) - *(*(ArrP + top2) + 1);
                for (short top3 = 0; top3 < N; top3++) {
                    if (top3 == top2|| top3 == top1) continue;
                    if (*(*(ArrP + top3) + 0) - deltaY == *(*(ArrP + top2) + 0) && *(*(ArrP + top3) + 1) - deltaX == *(*(ArrP + top2) + 1)) {
                        for (short top4 = 0; top4 < N; top4++) {
                            if (top4 == top3 || top4 == top2 || top4 == top1) continue;
                            if (*(*(ArrP + top4) + 0) + deltaX == *(*(ArrP + top3) + 0) && *(*(ArrP + top4) + 1) - deltaY == *(*(ArrP + top3) + 1)) {
                                if (!AddSquare(ArrS, ArrP, count_s, top1, top2, top3, top4)) {
                                    protocol << "Построен " << count_s << "-й квадрат с вершинами ";
                                    PrintCrd(protocol, ArrP, top1, top2, top3, top4); protocol << ".\n";
                                }
                            }
                            else {
                                protocol << "Не удалось построить " << count_s + 1 << "-й квадрат с вершинами ";
                                PrintCrd(protocol, ArrP, top1, top2, top3, top4); protocol << ".\n";
                            }
                        }
                    }
                }
            }
        }
    }
}

void AddArea(float** ArrA, short sqr1, short sqr2, float area, short& count) {
    
    if (sqr1 > sqr2) {
        short sqrn = sqr1;
        sqr1 = sqr2;
        sqr2 = sqrn;
    }
    short same = -1;
    for (short i = 0; i < count; i++) {
        if (*(*(ArrA + i) + 1) == sqr1 && *(*(ArrA + i) + 2) == sqr2) same = i;
    }
    if (same >= 0 && area > 0) {
        *(*(ArrA + same) + 0) = area;
        *(*(ArrA + same) + 1) = sqr1;
        *(*(ArrA + same) + 2) = sqr2;
    }
    else if (same >= 0 && (area == 0 || *(*(ArrA + same) + 0)>0)) return;
    else {
        *(*(ArrA + count) + 0) = area;
        *(*(ArrA + count) + 1) = sqr1;
        *(*(ArrA + count) + 2) = sqr2;
        count++;
    }
}

void CalсArea(ofstream& protocol, float*** ArrS, float** ArrA, short count_s, short& count_a) {

    for (short sqr1 = 0; sqr1 < count_s; sqr1++) {
        float tg1 = (*(*(*(ArrS + sqr1) + 5) + 0) - *(*(*(ArrS + sqr1) + 4) + 0)) / (*(*(*(ArrS + sqr1) + 4) + 1) - *(*(*(ArrS + sqr1) + 5) + 1));
        for (short sqr2 = 0; sqr2 < count_s; sqr2++) {
            float tg2 = (*(*(*(ArrS + sqr2) + 5) + 0) - *(*(*(ArrS + sqr2) + 4) + 0)) / (*(*(*(ArrS + sqr2) + 4) + 1) - *(*(*(ArrS + sqr2) + 5) + 1));
            if (sqr2 != sqr1 && tg2 == tg1) {
                float l = 0, w = 0;
                short pntnum[4]{}; short intrsctn = 0;
                for (short top = 0; top < 4; top++) {
                    if (*(*(*(ArrS + sqr1) + 0) + 0) < *(*(*(ArrS + sqr2) + top) + 0) && *(*(*(ArrS + sqr2) + top) + 0) < *(*(*(ArrS + sqr1) + 3) + 0)
                        && *(*(*(ArrS + sqr1) + 1) + 1) < *(*(*(ArrS + sqr2) + top) + 1) && *(*(*(ArrS + sqr2) + top) + 1) < *(*(*(ArrS + sqr1) + 0) + 1)) {
                        pntnum[intrsctn] = top; intrsctn++;
                    }
                }
                switch (intrsctn) {
                case 1: {
                    short p = (2 + pntnum[0]) % 4;
                    l = *(*(*(ArrS + sqr2) + pntnum[0]) + 0) - *(*(*(ArrS + sqr1) + p) + 0);
                    w = *(*(*(ArrS + sqr2) + pntnum[0]) + 1) - *(*(*(ArrS + sqr1) + p) + 1);
                    break;
                }
                case 2: {
                    if (pntnum[0] == 0 && pntnum[1] == 3 || pntnum[0] == 1 && pntnum[1] == 2) {
                        l = *(*(*(ArrS + sqr2) + 3) + 0) - *(*(*(ArrS + sqr2) + 0) + 0);
                        short p = fabs(pntnum[0] - 1);
                        w = *(*(*(ArrS + sqr2) + pntnum[0]) + 1) - *(*(*(ArrS + sqr1) + p) + 1);
                    }
                    else {
                        w = *(*(*(ArrS + sqr2) + 0) + 1) - *(*(*(ArrS + sqr2) + 1) + 1);
                        short p = fabs(pntnum[0] - 2);
                        l = *(*(*(ArrS + sqr2) + pntnum[0]) + 0) - *(*(*(ArrS + sqr1) + p) + 0);
                    }
                    break;

                }
                case 4: {
                    l = *(*(*(ArrS + sqr2) + 3) + 0) - *(*(*(ArrS + sqr2) + 0) + 0);
                    w = *(*(*(ArrS + sqr2) + 0) + 1) - *(*(*(ArrS + sqr2) + 1) + 1);
                    break;

                }
                }
                float area = round(fabs(l * w) * 1000) / 1000;
                AddArea(ArrA, sqr1, sqr2, area, count_a);
            }
        }
    }
    for (short i = 0; i < count_a; i++) {
        if (*(*(ArrA+i)+0) > 0) protocol << "Площадь пересечения " << *(*(ArrA + i) + 1) + 1 << "-го и "
            << *(*(ArrA + i) + 2) + 1 << "-го квадратов равна = " << *(*(ArrA + i) + 0) << "\n";
        else protocol << *(*(ArrA + i) + 1) + 1 << "-й и " << *(*(ArrA + i) + 2) + 1 << "-й квадраты не пересекаются.\n";
    }
}
int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "RU");

    ofstream result; result.open("result.txt");
    if (!result.is_open()) {
        cout << "Не удалось открыть файл для вывода результата.";
        return 1;
    }
    ofstream protocol; protocol.open("protocol.txt");
    if (!protocol.is_open()) {
        result << "Не удалось открыть протокол.";
        return 1;
        result.close();
    }
    ifstream points; points.open("points.txt");
    if (!points.is_open()) {
        result << "Не удалось открыть файл с входными данными.";
        return 1;
        result.close();
        protocol.close();
    }

    short N0 = 0; short N = 0;
    if (!CalcSize(points, result, N0)) {
        float** ArrP = new float* [N0];
        for (short i = 0; i < N0; i++) ArrP[i] = new float[2];

        GetPoints(points, protocol, ArrP, N0, N);

        float*** ArrS = new float** [4 * N];
        for (short i = 0; i < 4 * N; i++) {
            ArrS[i] = new float* [8];
            for (short j = 0; j < 8; j++) ArrS[i][j] = new float[2] {};
        }

        short count_s = 0;
        GetSquare(protocol, ArrP, ArrS, N, count_s);

        float** ArrA = new float* [count_s * count_s];
        for (short i = 0; i < count_s * count_s; i++) ArrA[i] = new float[3];

        short count_a = 0;
        CalсArea(protocol, ArrS, ArrA, count_s, count_a);
        PrintRes(ArrA, ArrS, count_s, count_a, N, result);

        for (short i = 0; i < count_s * count_s; i++) delete[] ArrA[i]; delete[] ArrA;
        for (short i = 0; i < 4 * N; i++) {
            for (short j = 0; j < 8; j++) delete[] ArrS[i][j];
            delete[] ArrS[i];
        } delete[] ArrS;
        for (short i = 0; i < N0; i++) delete[] ArrP[i];
        delete[] ArrP;

        result.close();
        protocol.close();
        points.close();
        return 0;
    }
}