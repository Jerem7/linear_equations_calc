// Jeremiasz Zolnierek-Kielczewski
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <limits>

using namespace std;

const double eps = 1e-9;

bool weryfikowany_int(int &num, const string &prompt = "Podaj liczbe: ") // weryfikacja wartosci calkowitych
{
    while (true)
    {
        cout << prompt;
        if (cin >> num)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return true;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "wprowadzono niepoprawny format danych. prosze wpisac liczbe" << endl;
    }
}
double weryfikowany_double(const string &prompt) // weryfikacja wartosci niecalkowitych
{
    double input;
    string input_str;
    while (true)
    {
        cout << prompt;
        if (cin >> input_str)
        {
            try
            {
                input = stod(input_str);
                return input;
            }
            catch (const std::invalid_argument &e)
            {
                cout << "wprowadzono niepoprawny format danych. prosze wpisac liczbe" << endl;
            }
        }
        else
        {
            cout << "wprowadzono niepoprawny format danych. prosze wpisac liczbe" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}
int wybor() // menu wyboru wersji obliczenia
{
    int wybranaMetoda;
    cout << "Prosze wybrac, jaka metoda ma byc rozwiazany uklad rownan liniowych?" << endl;
    cout << "1 dla metody dokladnej" << endl;
    cout << "2 dla metody iteracyjnej" << endl;
    while (!weryfikowany_int(wybranaMetoda, "Podaj wybrana opcje: "))
    {
        cout << "Nie podano poprawnej opcji" << endl;
    }
    if (wybranaMetoda == 1 || wybranaMetoda == 2)
    {
        return wybranaMetoda;
    }
    else
    {
        cout << "Nie podano poprawnej opcji" << endl;
        return wybor();
    }
}
void metoda_dokladna() // metoda dokladna, Eliminacja gaussa
{
    bool is_valid_input = false;
    int n;
    while (!is_valid_input || n <= 0)
    {
        is_valid_input = weryfikowany_int(n, "Podaj ilosc zmiennych w rownaniu: ");
        if (!is_valid_input || n <= 0)
        {
            cout << "Podano niepoprawna ilosc zmiennych.\n";
        }
    }
    vector<vector<double>> A(n, vector<double>(n));
    vector<double> b(n);
    cout << "Podaj wspolczynniki przy zmiennych i wyraz wolny w kazdym rownaniu:\n";
    for (int i = 0; i < n; ++i)
    {
        cout << "Rownanie " << i + 1 << ": ";
        for (int j = 0; j < n; ++j)
        {
            string message = "Podaj wspolczynnik a[" + to_string(i + 1) + "][" + to_string(j + 1) + "]: ";
            double a_ij = weryfikowany_double(message);
            A[i][j] = a_ij;
            if (!is_valid_input)
            {
                cout << "Podano niepoprawny wspolczynnik.\n";
                return;
            }
        }
        string message = "Podaj wyraz wolny b[" + to_string(i + 1) + "]: ";
        double b_i = weryfikowany_double(message);
        b[i] = b_i;
    }
    for (int i = 0; i < n; ++i)
    {
        int pivot = i;
        for (int j = i + 1; j < n; ++j)
        {
            if (abs(A[j][i]) > abs(A[pivot][i]))
            {
                pivot = j;
            }
        }
        swap(A[i], A[pivot]);
        swap(b[i], b[pivot]);
        if (abs(A[i][i]) < eps) // jezeli ktorys z diagonali jest mniejszy niz eps, wyswietlic komunikat i zakonczyc obliczenia
        {
            cout << "brak rozwiazan dla podanych danych\n";
            return;
        }
        for (int j = i + 1; j < n; ++j)
        {
            double coef = A[j][i] / A[i][i];
            for (int k = i; k < n; ++k)
            {
                A[j][k] -= coef * A[i][k];
            }
            b[j] -= coef * b[i];
        }
    }
    vector<double> x(n);
    for (int i = n - 1; i >= 0; --i)
    {
        double sum = 0;
        for (int j = i + 1; j < n; ++j)
        {
            sum += A[i][j] * x[j];
        }
        x[i] = (b[i] - sum) / A[i][i];
    }
    cout << "Rozwiazanie to:\n";
    for (int i = 0; i < n; ++i)
    {
        cout << "x[" << i + 1 << "] = " << x[i] << "\n";
    }
}
void metoda_iteracyjna()// metoda iteracyjna, metoda Jacobiego
{
    int n;
    do
    {
        weryfikowany_int(n, "Podaj ilosc zmiennych w rownaniu: ");
    } while (n <= 0);
    vector<vector<double>> A(n, vector<double>(n));
    vector<double> b(n);
    cout << "Podaj wspolczynniki przy zmiennych i wyraz wolny w kazdym rownaniu:\n";
    for (int i = 0; i < n; ++i)
    {
        cout << "Rownanie " << i + 1 << ": ";
        for (int j = 0; j < n; ++j)
        {
            A[i][j] = weryfikowany_double("Podaj wspolczynnik a[" + to_string(i + 1) + "][" + to_string(j + 1) + "]: ");
        }
        double b_i;
        b_i = weryfikowany_double("Podaj wyraz wolny b[" + to_string(i + 1) + "]: ");
        b[i] = b_i;
    }
    vector<double> x(n, 0);
    vector<double> x_prev(n);
    double eps_2 = 1e-6;
    double error = 1.0;
    int max_iterations;
    bool valid_input;
    do
    {
        valid_input = weryfikowany_int(max_iterations, "Podaj maksymalna ilosc iteracji: ");
    } while (!valid_input || max_iterations <= 0);
    int iterations = 0;
    while (error > eps_2 && iterations < max_iterations)
    {
        x_prev = x;
        for (int i = 0; i < n; ++i)
        {
            double sum = b[i];
            for (int j = 0; j < n; ++j)
            {
                if (i != j)
                {
                    sum -= A[i][j] * x_prev[j];
                }
            }
            x[i] = sum / A[i][i];
        }
        error = 0;
        for (int i = 0; i < n; ++i)
        {
            error += abs(x[i] - x_prev[i]);
        }
        iterations++;
    }
    if (iterations == max_iterations)
    {
        cout << "osiagnieto maksymalna ilosc iteracji bez uzyskania tolerowanego wyniku.\n"; // funkcja osiagnela maks iteracji bez osiagniecia tolerancji eps_2
    }
    else
    {
        cout << "Rozwiazanie to:\n";
        for (int i = 0; i < n; ++i)
        {
            cout << "x[" << i + 1 << "] = " << x[i] << "\n";
        }
    }
}
int main()
{
    int choice;
    do
    {
        int menu = wybor();
        if (menu == 1)
        {
            cout << "Wybrano metode dokladna" << endl;
            metoda_dokladna();
        }
        else if (menu == 2)
        {
            cout << "Wybrano metode iteracyjna" << endl;
            metoda_iteracyjna();
        }
        do
        {
            cout << "Czy chcesz kontynuowac? (1 - Powrot do poczatku, 0 - Zamknij program): ";
            if (!(cin >> choice))
            {                                                        
                cin.clear();                                         
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            }
        } while (choice != 0 && choice != 1); 
    } while (choice != 0);
    return 0;
}