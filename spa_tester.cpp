
/////////////////////////////////////////////
//          SPA TESTER for SPA.C           //
//                                         //
//      Solar Position Algorithm (SPA)     //
//                   for                   //
//        Solar Radiation Application      //
//                                         //
//             August 12, 2004             //
//                                         //
//   Filename: SPA_TESTER.C                //
//                                         //
//   Afshin Michael Andreas                //
//   afshin_andreas@nrel.gov (303)384-6383 //
//                                         //
//   Measurement & Instrumentation Team    //
//   Solar Radiation Research Laboratory   //
//   National Renewable Energy Laboratory  //
//   1617 Cole Blvd, Golden, CO 80401      //
/////////////////////////////////////////////

/////////////////////////////////////////////
// This sample program shows how to use    //
//    the SPA.C code.                      //
/////////////////////////////////////////////
#include <iostream>
#include <windows.h>  //almers
#include <stdlib.h>
#include <math.h>  // almers
#include <stdio.h>
#include "spa.h"  //include the SPA header file



int main (int argc, char *argv[])
{
    spa_data spa;  //declare the SPA structure

    int result;
    float min, sec;
    float alpha,teta;
    float gama,gama1;
    float  PI=3.14159265358979323;
    float test,H,A;


/////////////////////////////////////////////////////////////////////////////
/////  données géométriques pour la position du miroire et du récepteur /////
/////                                                                   /////
/////                Xm: abscisse horizontal du miroire                 /////
/////                Zm: hauteur du miroir (par défaut en prend Zm=0)   /////
/////                Xr:  abscisse horizontal du récepteur              /////
/////                Zr:  hateur du récepteur                           /////
/////                                                                   /////
                                    float Xm=0;                         /////
                                    float Zm=0;                         /////
                                    float Xr=0;                         /////
                                    float Zr=5;                         /////
/////                                                                   /////
/////                                                                   /////
/////////////////////////////////////////////////////////////////////////////



float deltat=60;  //       pas de temps pour le calcul des la position du soleil

//            Aqcuisition du temps à partir de Windows
//            faire attension car en utilisant un autre système d'exploitation,
//            il faut changer la librairie <windows.h> dans les déclarations en haut)

SYSTEMTIME t;
GetSystemTime(&t);


//enter required input values into SPA structure

 //   spa.year          = 2014;
//    spa.month         = 6;
//    spa.day           = 20;
//    spa.hour          = 12;
//    spa.minute        = 30;
//    spa.second        = 0;



    spa.timezone      = +1; //almers: à changer lorsqu'on retourne à l'heure d'hiver
    spa.delta_ut1     = 0;
    spa.delta_t       = 67;
    spa.longitude     = -5.5396; //almers: for Benguerir
    spa.latitude      = 33.8916; //almers: for Benguerir
    spa.elevation     = 540;   //almers: for Benguerir
    spa.pressure      = 820;
    spa.temperature   = 27; //almers: for Benguerir
    spa.slope         = 30;
    spa.azm_rotation  = -10;
    spa.atmos_refract = 0.5667;
    spa.function      = SPA_ALL;



//============introduire les boucles pour la régulation....

    int actual_hour = t.wHour;      // cette instruction doit être cahngée en fonction du système d'exploitation
    int actual_min  = t.wMinute;    // cette instruction doit être cahngée en fonction du système d'exploitation
    int actual_sec  = t.wSecond;    // cette instruction doit être cahngée en fonction du système d'exploitation
    int actual_day  = t.wDay;       // cette instruction doit être cahngée en fonction du système d'exploitation
    int actual_year  = t.wYear;     // cette instruction doit être cahngée en fonction du système d'exploitation
    int actual_month  = t.wMonth;     // cette instruction doit être cahngée en fonction du système d'exploitation

    spa.year          = actual_year;
    spa.month         = actual_month;
    spa.day           = actual_day ;
    spa.hour          = actual_hour;
    spa.minute        = actual_min;
    spa.second        = actual_sec;



    //call the SPA calculate function and pass the SPA structure

    result = spa_calculate(&spa);

    if (result == 0)  //check for SPA errors
    {


//  calcul de langle teta d'inclinaison du miroir

H=(90-spa.zenith)*PI/180;
A=(spa.azimuth-180)*PI/180;

alpha=atan(cos(A)*(tan(A)/tan(H)));

/*=====================================*/

if (Xm < Xr)  { test = PI/2;}

                else

{

         if (Xm > Xr)  { test =- PI/2;}}
         gama1 = atan((Zr-Zm)/(Xr-Xm));
         gama=(gama1* 180)/PI;
         teta = (0.5) * (test-fabs(alpha)-gama1);

                if(alpha>0)

         { teta = (0.5) * (test+alpha-gama1);}

                teta= 90+45+teta*360/(2*PI);

//========================================*/
        //display the results inside the SPA structure

        printf("Julian Day:    %.6f\n",spa.jd);
        printf("L:             %.6e degrees\n",spa.l);
        printf("B:             %.6e degrees\n",spa.b);
        printf("R:             %.6f AU\n",spa.r);
        printf("H:             %.6f degrees\n",spa.h);
        printf("Delta Psi:     %.6e degrees\n",spa.del_psi);
        printf("Delta Epsilon: %.6e degrees\n",spa.del_epsilon);
        printf("Epsilon:       %.6f degrees\n",spa.epsilon);
        printf("Zenith:        %.6f degrees\n",90-spa.zenith); // almers
        printf("Azimuth:       %.6f degrees\n",spa.azimuth-180);  // almers
        printf("Incidence:     %.6f degrees\n",spa.incidence);

        min = 60.0*(spa.sunrise - (int)(spa.sunrise));
        sec = 60.0*(min - (int)min);
        printf("Sunrise:       %02d:%02d:%02d Local Time\n", (int)(spa.sunrise), (int)min, (int)sec);

        min = 60.0*(spa.sunset - (int)(spa.sunset));
        sec = 60.0*(min - (int)min);
        printf("Sunset:        %02d:%02d:%02d Local Time\n", (int)(spa.sunset), (int)min, (int)sec);
        printf(" orientation angle teta: %.6f degrees (for the referenced mirror)\n", teta); //almers: afichage de Teta en degree

    } else printf("SPA Error Code: %d\n", result);

    system("PAUSE");

    return 0;
}

/////////////////////////////////////////////
// The output of this program should be:
//
//Julian Day:    2452930.312847
//L:             2.401826e+01 degrees
//B:             -1.011219e-04 degrees
//R:             0.996542 AU
//H:             11.105902 degrees
//Delta Psi:     -3.998404e-03 degrees
//Delta Epsilon: 1.666568e-03 degrees
//Epsilon:       23.440465 degrees
//Zenith:        50.111622 degrees
//Azimuth:       194.340241 degrees
//Incidence:     25.187000 degrees
//Sunrise:       06:12:43 Local Time
//Sunset:        17:20:19 Local Time
//
/////////////////////////////////////////////
