#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define E_H -0.4997908 // em Hartree

double Energy_11(double radius) {
	double value = ( (1/radius) * ( 1-exp(-2*radius) * (1+radius) ) );
	return value;
}

double Energy_12(double radius) {
	double value = ( exp(-radius) * (1+radius) );
	return value;
}

double Overlap_12(double radius){
	double value = (exp(-radius)) * (1+radius+((radius*radius)/3) );
	return value;
}

double Energy_1(double radius, double E_11, double E_12, double S_12){
	double value = E_H + (1/radius) + ( (E_11-E_12) / (1-S_12) );
	return value;
}

double Energy_2(double radius, double E_11, double E_12, double S_12){
	double value = E_H + (1/radius) - ( (E_11+E_12) / (1+S_12) );
	return value;
}

double minimal_element_array(double *number, int n) {
    double temp = 0;
    int j, i;
    for (i = 1; i < n; i++) {
        for (j = 0; j < n - i; j++) {
            if (number[j] > number[j + 1]) {
                temp = number[j];
                number[j] = number[j + 1];
                number[j + 1] = temp;
            }
        }
    }
    return temp;
}

int main() {
	double R_initial, R_final, R_increment, Anti_Bonding, Bonding, E11, E12, S12;
	printf("\nQual o valor inicial da distância entre os Hidrogênios? ");
	scanf ("%lf", &R_initial);
	printf("\nQual o valor final da distância entre os Hidrogênios? ");
	scanf ("%lf", &R_final);
	printf("\nQual o valor incremental entre a distância inicial e final? ");
	scanf ("%lf", &R_increment);
	FILE* f;
	f = fopen("h2+.energy", "w+");
	fprintf(f, "R 		E1 		E2\n\n");
	int list_length = (R_final - R_initial) / R_increment;
	int counter = 0;
	double list_energy[list_length];
	double list_radius[list_length];
	for (double R = R_initial; R <= R_final; R+=R_increment) {
		E11 = Energy_11(R);
		E12 = Energy_12(R);
		S12 = Overlap_12(R);
		Anti_Bonding = Energy_1(R, E11, E12, S12);
		Bonding = Energy_2(R, E11, E12, S12);
		fprintf(f, "%.3f		%.4f		%.4f\n", R, Anti_Bonding, Bonding);
		list_energy[counter] = Bonding;
		list_radius[counter] = R;
		counter = counter + 1;
	}
	double temp_list[list_length];
	for (int h = 0; h < list_length; h++) {
		temp_list[h] = list_energy[h];
	}
	double minimal_energy = minimal_element_array(temp_list, list_length);
	double minimal_radius;
	for (int f = 0; f < list_length; f++) {
		if (minimal_energy == list_energy[f]) {
			minimal_radius = list_radius[f];
		}
	}
	char minimal_energy_char[10];
	sprintf(minimal_energy_char,"%.4f",minimal_energy);
	char comando[2000] = "";
	strcat(comando, "gnuplot -e \'set term png transparent size 2560,1920; set xrange [-1:20]; set yrange[:1.5]; De = ");
	strcat(comando, minimal_energy_char);
	strcat(comando, ";set ylabel \"Energia (Hartree)\" font \"Verdana,35\" offset -7,0,0; set xlabel \"Distância entre Núcleos (Bohr)\" font \"Verdana,35\" offset 0,-3,0; set lmargin screen 0.08; set rmargin screen 0.98; set key font \"Verdana,40\";set output \"energy_H2plus.png\"; plot \"h2+.energy\" using 1:2 with line linewidth 5.000 lc 2 title \"Anti-Ligante\", \"h2+.energy\" using 1:3 with line linewidth 5.000 lc 6 title \"Ligante\"\', \"ponto.ponto\" using 1:2");
	system(comando);
	printf("%s\n", comando);
	printf("Termindo, as energias foram impressas no arquivo \'h2+.energy\' e o plot no arquivo \'energy_H2plus.png\'\n");
}
