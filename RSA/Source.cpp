#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

int gcd(int a, int b) {
    int t;
    while (1) {
        t = a % b;
        if (t == 0)
            return b;
        a = b;
        b = t;
    }
}

void SieveOfEratosthenes(int n)
{
    // Create a boolean array
    // "prime[0..n]" and initialize
    // all entries it as true.
    // A value in prime[i] will
    // finally be false if i is
    // Not a prime, else true.
    bool* prime = new bool[n + 1];
    size_t Size = sizeof(bool) * (n + 1);
    memset(prime, true, Size);

    for (int p = 2; p * p <= n; p++)
    {
        // If prime[p] is not changed,
        // then it is a prime
        if (prime[p] == true)
        {
            // Update all multiples
            // of p greater than or
            // equal to the square of it
            // numbers which are multiple
            // of p and are less than p^2
            // are already been marked.
            for (int i = p * p; i <= n; i += p)
                prime[i] = false;
        }
    }

    // Print all prime numbers
    for (int p = 2; p <= n; p++)
        if (prime[p])
            cout << p << " ";
}

int main()
{
	wcout << L"Please type in a String to encrypt: ";

	wstring ToEncrypt;
	wcin >> ToEncrypt;

    SieveOfEratosthenes(270);

    double p = 251;
    double q = 257;
    double n = p * q;
    double phi = (p - 1) * (q - 1);

    double track;
    double PublicKey = 12;
    while (PublicKey < phi) {
        track = gcd(PublicKey, phi);
        if (track == 1)
            break;
        else
            PublicKey++;
    }

    double PrivateKey1 = 1 / PublicKey;
    double PrivateKey = fmod(PrivateKey1, phi);

    int Len = static_cast<int>(ToEncrypt.size());
    double* NewMessage = new double[Len];
    int i = 0;
    for (char CurrentT : ToEncrypt)
    {
        double AsDouble = static_cast<double>(CurrentT);
        double Encrypted = pow(AsDouble, PublicKey);

        NewMessage[i] = Encrypted;
        i++;
    }

    wcout << endl << L"Encrypted Message: ";
    for (int i = 0; i < Len; i++)
        wcout << NewMessage[i] << L" ";
    wcout << endl << endl;

    char* Decrypted = new char[Len];
    for (int i = 0; i < Len; i++)
        Decrypted[i] = static_cast<char>(pow(NewMessage[i], PrivateKey));

    wcout << L"Decrypted Message: ";
    for (int i = 0; i < Len; i++)
        wcout << Decrypted[i];
}