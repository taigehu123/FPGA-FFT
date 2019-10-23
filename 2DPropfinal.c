#include "xfft_v9_0_bitacc_cmodel.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <complex>
#include <cmath>


using namespace std;

int main(int argc, char **argv)
{
  // Define constants
  const std::complex<double> j(0.0, 1.0);
  const double pi = 3.141592653589793;
  // Define Sample Size
  const int dimensions = atoi(argv[1]);
  std::string input(argv[2]);

  // Generics for this smoke test
  // (Any legal combination should work)
  const int C_NFFT_MAX      = log2(dimensions);
  const int C_ARCH          = 3;
  const int C_USE_FLT_PT    = 1;
  const int C_HAS_NFFT      = 0;
  const int C_INPUT_WIDTH   = 32;
  const int C_TWIDDLE_WIDTH = 25;
  const int C_HAS_SCALING   = 0;
  const int C_HAS_BFP       = 0;
  const int C_HAS_ROUNDING  = 0;

  // Handle multichannel FFTs if required
  const int channels = 1;

  // Declare generic struct and set to generics to test
  struct xilinx_ip_xfft_v9_0_generics generics;
  generics.C_NFFT_MAX      = C_NFFT_MAX;
  generics.C_ARCH          = C_ARCH;
  generics.C_USE_FLT_PT    = C_USE_FLT_PT;
  generics.C_HAS_NFFT      = C_HAS_NFFT;
  generics.C_INPUT_WIDTH   = C_INPUT_WIDTH;
  generics.C_TWIDDLE_WIDTH = C_TWIDDLE_WIDTH;
  generics.C_HAS_SCALING   = C_HAS_SCALING;
  generics.C_HAS_BFP       = C_HAS_BFP;
  generics.C_HAS_ROUNDING  = C_HAS_ROUNDING;

  // Create FFT state
  struct xilinx_ip_xfft_v9_0_state* state = xilinx_ip_xfft_v9_0_create_state(generics);
  if (state == NULL) {
    cerr << "ERROR: could not create FFT state object" << endl;
    return 1;
  }

  // Create structure for FFT inputs and outputs
  struct xilinx_ip_xfft_v9_0_inputs  inputs;
  struct xilinx_ip_xfft_v9_0_outputs outputs;
  // point size
  inputs.nfft = C_NFFT_MAX;

  double xn_re[dimensions];
  double xn_im[dimensions];
  double xk_re[dimensions];
  double xk_im[dimensions];

  float result_re[dimensions][dimensions];
  float result_im[dimensions][dimensions];

  inputs.xn_re       = &xn_re[0];
  inputs.xn_re_size  = dimensions;
  inputs.xn_im       = &xn_im[0];
  inputs.xn_im_size  = dimensions;

  outputs.xk_re      = &xk_re[0];
  outputs.xk_re_size = dimensions;
  outputs.xk_im      = &xk_im[0];
  outputs.xk_im_size = dimensions;

  /* Shift for plotting; Declare shift matrix
  double Mx = dimensions;
  double Tx = (Mx + 1) / 2;
  complex<double>  shift[dimensions];
  complex<double> ishift[dimensions];

  for (int i = 0; i < Mx; i++) {
     shift[i] = exp(j * 2.0 * pi * Tx * (i + Tx) * (-1 / Mx));
    ishift[i] = exp(j * 2.0 * pi * Tx * (i + Tx) * ( 1 / Mx));
  }*/

  ofstream  infile, isfile;
  infile.open("2dinput.txt",        ios::out|ios::binary);
  isfile.open("2dinput_shift.txt",  ios::out|ios::binary);

  ofstream ftfile, fsfile, fvfile;
  ftfile.open("2dfftabs.txt",       ios::out);
  fsfile.open("2dfftabs_shift.txt", ios::out);
  fvfile.open("2dProp.txt",         ios::out);

  // Initialize Intermediatary Structure
  float inter_re[dimensions][dimensions];
  float inter_im[dimensions][dimensions];

  // Set direction to forward
  inputs.direction = 1;

  float val;
  int   row;
  int   col;

  start:

  std::ifstream myFile(input);
  if (myFile.is_open()) {
    for (int i = 0; i < dimensions; i++) {
      for (int n = 0; n < dimensions; n++) {
        if (n < 256) {
          col = n + 256;
        } else {
          col = n - 256;
        }

        if (i < 256) {
          row = i + 256;
        } else {
          row = i - 256;
        }

        if (inputs.direction == 1) {
          myFile >> xn_re[col];
          myFile >> xn_im[col];
          val = pow(pow(xn_re[col], 2) + pow(xn_im[col], 2), .5);
          infile << val << endl;
        } else { 
          xn_re[col] = result_re[i][n];
          xn_im[col] = result_im[i][n];
        }
      }

      // Simulate the FFT
      if (xilinx_ip_xfft_v9_0_bitacc_simulate(state, inputs, &outputs) != 0) {
        cerr << "ERROR: simulation did not complete successfully" << endl;
        // Destroy the FFT state to free up memory
        xilinx_ip_xfft_v9_0_destroy_state(state);
        return 1;
      } else {
        cout << "Simulation completed successfully, " << "Line " << i << endl;
        for (int n = 0; n < dimensions; n++) {
          if (inputs.direction == 1) {
            inter_re[n][row] = xk_re[n];
            inter_im[n][row] = xk_im[n];
          } else {
            inter_re[n][row] = xk_re[n] / dimensions;
            inter_im[n][row] = xk_im[n] / dimensions;
          }
        }
      }
    }
  }

  infile.close();

  //Define Propogation Parameters
  double Fsize = 3.9 * 1E-3;
  double Mx = dimensions;
  double My = dimensions;
  double k0 = 2 * pi / (1.24 * 1E-6/ 1400) ;
  double dx = Fsize * k0 / Mx;
  double dy = Fsize * k0 / My;
  double dkx = 2 * pi / (Mx * dx);
  double dky = 2 * pi / (My * dy);
  double z = 73.8320*k0;

  double kx[dimensions];
  
  for (int i = 1; i < dimensions; i++) {
    kx[i] = dkx * (i - Mx / 2);
  }

  double K_square;
 
  // Initialize Result
  std::complex<double> temp;
  
  
  // Second-Step: FFT on each column
  for (int i = 0; i < dimensions; i++) {
    for (int n = 0; n < dimensions; n++) {
      xn_re[n] = inter_re[i][n];
      xn_im[n] = inter_im[i][n];
    }

    if (xilinx_ip_xfft_v9_0_bitacc_simulate(state, inputs, &outputs) != 0) {
      cerr << "ERROR: simulation did not complete successfully" << endl;
      // Destroy the FFT state to free up memory
      xilinx_ip_xfft_v9_0_destroy_state(state);
      return 1;
    } else {
      cout << "Simulation completed successfully, " << "Column " << i << endl;
      float value;
      for (int n = 0; n < dimensions; n++) {
        if (n < 256) {
          col = n + 256;
        } else {
          col = n - 256;
        }

        if (i < 256) {
          row = i + 256;
        } else {
          row = i - 256;
        }

        if (inputs.direction == 1) {
          K_square = z * (pow(kx[col], 2) + pow(kx[row], 2));
	  temp = complex<double>(xk_re[n], xk_im[n]);
          temp = temp * exp(-j * K_square / complex<double>(2.0, 0.0));

	  result_re[row][col] = real(temp); 
          result_im[row][col] = imag(temp);
          
          /*result_re[row][col] = xk_re[n];
          result_im[row][col] = xk_im[n];*/
          
        } else {
          result_re[row][col] = xk_re[n] / dimensions; 
          result_im[row][col] = xk_im[n] / dimensions;
        }
      }
    }
  }
  
  if (inputs.direction == 1) {
    for (int i = 0; i < dimensions; i++) {
      for (int n = 0; n < dimensions; n++) {
        fsfile << pow(pow(result_re[i][n], 2) + pow(result_im[i][n], 2), .5) << endl;
      }
    }

    inputs.direction = 0;
    goto start;
  }
  
  for (int i = 0; i < dimensions; i++) {
    for (int n = 0; n < dimensions; n++) {
      fvfile << (pow(pow(result_re[i][n], 2) + pow(result_im[i][n], 2), .5)) << endl;
    }
  }

  fvfile.close();
  ftfile.close();
  fsfile.close();

  // Destroy the FFT state to free up memory
  xilinx_ip_xfft_v9_0_destroy_state(state);
}

