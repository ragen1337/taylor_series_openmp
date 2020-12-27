#include <stdio.h>
#include <math.h>
#include <omp.h>

long double my_sh(long double x, float n);
long double my_exp(long double x, float n);
long double my_exp_from_squared_x(long double x, float n);

/*
My function: sh(x)^2 * e^(x^2)
*/
int main (void)
{
    float num = 100000000;

    long double squared_ld_x;
    long double math_h_values_array[10];
    long double my_values_array[10];
    long double math_h_sinh_value;
    long double my_sinh_value;
    long double x;

    int threads_amount;
    printf("Enter amount of threads: ");
    scanf("%d", &threads_amount);

    float start, end, delta;

    printf("FOR %i THREADS:\n", threads_amount);
    
    start = omp_get_wtime();

    #pragma omp parallel for num_threads(threads_amount) private(x, squared_ld_x, math_h_sinh_value, my_sinh_value)
        for(int i = 1; i <= 10; i++){
            x = 0.5 * (long double)i;
            squared_ld_x = x*x;
            math_h_sinh_value = sinhl(x);
            math_h_values_array[i-1] =  math_h_sinh_value * math_h_sinh_value * expl(squared_ld_x);

            my_sinh_value = my_sh(x, num);
            my_values_array[i-1] = my_sinh_value * my_sinh_value * my_exp(squared_ld_x, num);
        }

    end = omp_get_wtime();
    delta = end - start;

    printf("algorithm duration: %f\n\n", delta );

    for(short i = 0; i < 10; i++){
        if( i == 3 )
            i = 7;
        
        printf("\nFOR X == %f\n", (float)(i+1)/2);
        printf("MATH.H FUNCTION: %Le \n", math_h_values_array[i]);
        printf("MY FUNCTION: %Le \n", my_values_array[i]);
        printf("ERROR BETWEEN MY AND MATH FUNCTIONS: %Le \n", math_h_values_array[i] - my_values_array[i]);
    }

    printf("****************************************************\n");
}

/*
My hyperbolic sinus taylor series
*/
long double my_sh(long double x, float n)
{
    long double previous = x;
    long double sum = x;

    float squared_x = x*x;
    for(int i = 1; i < n; i++){
        previous *=  squared_x/(long double)(4*i*i + 2*i);
        sum += previous;
    }

    return sum;
}

/*
My exponent taylor series
*/
long double my_exp(long double x, float n)
{
    long double previous = 1;
    long double sum = 1;

    for(int i = 0; i < n; i++){
        previous *=  x/(long double)(i + 1);
        sum += previous;
    }

    return sum;
}
