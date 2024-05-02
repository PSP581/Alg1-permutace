#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Funkce pro výpis permutace
void printPermutation(const vector<int> &permutation)
{
    for (int i = 0; i < permutation.size(); ++i)
    {
        cout << permutation[i] << " ";
    }
    cout << endl;
}

// Funkce pro nalezení největšího mobilního prvku
int findLargestMobile(const vector<int> &permutation, const vector<int> &directions)
{
    int largestMobile = 0;
    for (int i = 0; i < permutation.size(); ++i)
    {
        if ((directions[i] == -1 && i != 0 && permutation[i] > permutation[i - 1]) ||
            (directions[i] == 1 && i != permutation.size() - 1 && permutation[i] > permutation[i + 1]))
        {
            largestMobile = max(largestMobile, permutation[i]);
        }
    }
    return largestMobile;
}

// Funkce pro otočení směru šipek
void toggleDirections(vector<int> &permutation, vector<int> &directions, int pivot)
{
    for (int i = 0; i < permutation.size(); ++i)
    {
        if (permutation[i] > pivot)
        {
            directions[i] *= -1;
        }
    }
}

// Implementace Johnson-Trotterova algoritmu pro generování permutací
vector<vector<int>> generatePermutations(int n)
{
    vector<vector<int>> permutations;
    vector<int> permutation(n), directions(n, -1);

    // Inicializace permutace
    for (int i = 0; i < n; ++i)
    {
        permutation[i] = i + 1;
    }

    // Přidání první permutace
    permutations.push_back(permutation);

    while (true)
    {
        int mobile = findLargestMobile(permutation, directions);
        if (mobile == 0)
            break;

        // Nalezení indexu mobilního prvku
        int index;
        for (index = 0; index < n; ++index)
        {
            if (permutation[index] == mobile)
                break;
        }

        // Výměna mobilního prvku s prvkem, na který ukazuje jeho šipka
        if (directions[index] == -1)
        {
            swap(permutation[index], permutation[index - 1]);
            swap(directions[index], directions[index - 1]);
        }
        else
        {
            swap(permutation[index], permutation[index + 1]);
            swap(directions[index], directions[index + 1]);
        }

        // Otočení směru šipek pro prvky větší než mobilní prvek
        toggleDirections(permutation, directions, mobile);

        // Přidání nové permutace do seznamu
        permutations.push_back(permutation);
    }

    return permutations;
}

int main()
{
    int n;
    cout << "Zadejte velikost permutace: ";
    cin >> n;

    // Generování permutací pomocí Johnson-Trotter algoritmu
    auto startJT = high_resolution_clock::now();
    vector<vector<int>> permutationsJT = generatePermutations(n);
    auto stopJT = high_resolution_clock::now();
    auto durationJT = duration_cast<milliseconds>(stopJT - startJT);

    // Výpis vygenerovaných permutací pomocí Johnson-Trotter algoritmu
    cout << "Permutace generovane pomoci Johnson-Trotter algoritmu:" << endl;
    for (const auto &permutation : permutationsJT)
    {
        printPermutation(permutation);
    }

    cout << "Permutace generované pomocí STD" << endl;
    // Generování permutací pomocí std::next_permutation
    vector<int> permutation(n);
    for (int i = 0; i < n; ++i)
    {
        permutation[i] = i + 1;
    }

    auto startNP = high_resolution_clock::now();
    do
    {
        printPermutation(permutation);
    } while (next_permutation(permutation.begin(), permutation.end()));
    auto stopNP = high_resolution_clock::now();
    auto durationNP = duration_cast<milliseconds>(stopNP - startNP);

    cout << "Cas generovani pomoci Johnson-Trotter algoritmu: " << durationJT.count() << " ms" << endl;
    cout << "Cas generovani pomoci std::next_permutation: " << durationNP.count() << " ms" << endl;

    return 0;
}