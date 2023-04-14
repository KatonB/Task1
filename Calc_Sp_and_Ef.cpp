// Вычисляем ускорение и эффективность
#include <iostream>
using namespace std;
int main(){
    const int num_threads = 8;
    //double StaticTime[num_threads] = {964, 483, 314, 247, 320, 285, 262, 250};
    double StaticTime[num_threads] = {893, 458, 317, 271, 247, 243, 240, 247};
    double SpeedUp[num_threads];
    double Efficiency[num_threads];
    for (int i = 0; i<num_threads; i++){
        SpeedUp[i] = StaticTime[0]/StaticTime[i];
        Efficiency[i] = SpeedUp[i]/double(i+1);
        cout << Efficiency[i] << ',' << '\n';
    }


    return 0;
}