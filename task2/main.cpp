#include <iostream>
#include <math.h>

using namespace std;


// Function to calculate the greatest common divisor
int gcd(int a, int h) {
    int temp;
    while (1) {
        temp = a % h;
        if (temp == 0)
            return h;
        a = h;
        h = temp;
    }
}

// function to produce the main components of the private and public keys
// those being e , d and n
int generateKeys(double& e, double& d, double& n) {
    // p and q are prime numbers that should be very large
    double p = 3;
    double q = 7;

    n = p * q;
    double not_n = (p - 1) * (q - 1);

    e = 2;
    // while loop finds the e 
    // e and not_n have a gcd = 1
    while (e < not_n) {
        double count = gcd(e, not_n);
        if (count == 1){
            // gcd of 1 is found
            break;
        }
        else
            e++;
    }

    double k = 2;
    d = (1 + (k * not_n)) / e;

    // checks if d is an integer, if not increment k and recalculate d
    while (d - floor(d) != 0) {
        k++;
        d = (1 + (k * not_n)) / e;
    }


    return 0;
}

double encrypt(double msg, double e, double n) {
    double c = pow(msg, e); // raises msg to the power of e
    c = fmod(c, n);
    return c;
}

double decrypt(double c, double d, double n) {
    double m = pow(c, d);  // raises c to the power of d
    m = fmod(m, n);
    return m;
}

int main() {

    double e, d, n;
    generateKeys(e, d, n);

    
    double msg = 16;
    double c = encrypt(msg, e, n);
    double m = decrypt(c, d, n);

    cout<<"Message = "<<msg<<endl;
    cout<<"n = "<<n<<endl;
    cout<<"e = "<<e<<endl;
    cout<<"d = "<<d<<endl;
    cout<<"Encrypted message = "<<c<<endl;
    cout<<"Decrypted Message = "<<m<<endl;

    return 0;
}