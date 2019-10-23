// $Id: run_bitacc_cmodel.c,v 1.1.2.4 2007/10/09 09:59:13 ccleg Exp $
//
//  Copyright(C) 2007 by Xilinx, Inc. All rights reserved.
//  This text/file contains proprietary, confidential
//  information of Xilinx, Inc., is distributed under license
//  from Xilinx, Inc., and may be used, copied and/or
//  disclosed only pursuant to the terms of a valid license
//  agreement with Xilinx, Inc.  Xilinx hereby grants you
//  a license to use this text/file solely for design, simulation,
//  implementation and creation of design files limited
//  to Xilinx devices or technologies. Use with non-Xilinx
//  devices or technologies is expressly prohibited and
//  immediately terminates your license unless covered by
//  a separate agreement.
//
//  Xilinx is providing this design, code, or information
//  "as is" solely for use in developing programs and
//  solutions for Xilinx devices.  By providing this design,
//  code, or information as one possible implementation of
//  this feature, application or standard, Xilinx is making no
//  representation that this implementation is free from any
//  claims of infringement.  You are responsible for
//  obtaining any rights you may require for your implementation.
//  Xilinx expressly disclaims any warranty whatsoever with
//  respect to the adequacy of the implementation, including
//  but not limited to any warranties or representations that this
//  implementation is free from claims of infringement, implied
//  warranties of merchantability or fitness for a particular
//  purpose.
//
//  Xilinx products are not intended for use in life support
//  appliances, devices, or systems. Use in such applications are
//  expressly prohibited.
//
//  This copyright and support notice must be retained as part
//  of this text at all times. (c) Copyright 2007 Xilinx, Inc.
//  All rights reserved.
//-------------------------------------------------------------------

#include "xfft_v5_0_bitacc_cmodel.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main()
{

  // Generics for this smoke test
  // (Any legal combination should work)
  const int C_NFFT_MAX      = 10;
  const int C_ARCH          = 1;
  const int C_HAS_NFFT      = 0;
  const int C_INPUT_WIDTH   = 16;
  const int C_TWIDDLE_WIDTH = 16;
  const int C_HAS_SCALING   = 1;
  const int C_HAS_BFP       = 0;
  const int C_HAS_ROUNDING  = 0;

  // Handle multichannel FFTs if required
  const int channels = 1;

  // Declare generic struct and set to generics to test
  struct xilinx_ip_xfft_v5_0_generics generics;
  generics.C_NFFT_MAX      = C_NFFT_MAX;
  generics.C_ARCH          = C_ARCH;
  generics.C_HAS_NFFT      = C_HAS_NFFT;
  generics.C_INPUT_WIDTH   = C_INPUT_WIDTH;
  generics.C_TWIDDLE_WIDTH = C_TWIDDLE_WIDTH;
  generics.C_HAS_SCALING   = C_HAS_SCALING;
  generics.C_HAS_BFP       = C_HAS_BFP;
  generics.C_HAS_ROUNDING  = C_HAS_ROUNDING;

  // Create FFT state
  struct xilinx_ip_xfft_v5_0_state* state = xilinx_ip_xfft_v5_0_create_state(generics);
  if (state == NULL) {
    cerr << "ERROR: could not create FFT state object" << endl;
    return 1;
  }

  // Create structure for FFT inputs and input data arrays
  struct xilinx_ip_xfft_v5_0_inputs inputs;
  // point size
  inputs.nfft = C_NFFT_MAX;
  const int samples = 1 << C_NFFT_MAX;
  double xn_re[samples];
  double xn_im[samples];
  inputs.xn_re = &xn_re[0];
  inputs.xn_re_size = samples;
  inputs.xn_im = &xn_im[0];
  inputs.xn_im_size = samples;

  // Create structure for FFT outputs and output data arrays
  struct xilinx_ip_xfft_v5_0_outputs outputs;
  double xk_re[samples];
  double xk_im[samples];
  outputs.xk_re = &xk_re[0];
  outputs.xk_re_size = samples;
  outputs.xk_im = &xk_im[0];
  outputs.xk_im_size = samples;

  // Loop through channels in a multichannel FFT, if required
  bool all_ok = true;
  for (int c=1; c<=channels; c++) {
    string channel_text;
    if (channels > 1) {
      ostringstream c_str;
      c_str << c;
      channel_text = " for channel " + c_str.str();
    }

    // Create input data frame: constant data
    double constant_input = 0.5;
    int i;
    for (i=0; i<samples; i++) {
      xn_re[i] = constant_input;
      xn_im[i] = 0.0;
    }

    // Set scaling schedule to 1/N : 2 in each stage for radix-4 / streaming, 1 in each stage for radix-2 [Lite]
    const int stages = (C_ARCH == 1 || C_ARCH == 3) ? (C_NFFT_MAX+1)/2 : C_NFFT_MAX;
    const int scaling = (C_ARCH == 1 || C_ARCH == 3) ? 2 : 1;
    int scaling_sch[stages];
    for (i=0; i<stages; i++) {
      scaling_sch[i] = scaling;
    }
    inputs.scaling_sch = &scaling_sch[0];
    inputs.scaling_sch_size = stages;

    // Set direction to forward
    inputs.direction = 1;

    // Simulate the FFT
    cout << "Running the C model" << channel_text << "..." << endl;
    if (xilinx_ip_xfft_v5_0_bitacc_simulate(state, inputs, &outputs) != 0) {
      cerr << "ERROR: simulation did not complete successfully" << endl;
      return 1;
    } else {
      cout << "Simulation completed successfully" << endl;
    }

    // Check outputs are correct
    // The FFT of constant input data is an impulse
    // Therefore all output samples should be zero except for the first
    // The value of the first sample depends on the type of scaling used
    bool ok = true;

    // Check xk_re_size and xk_im_size
    if (outputs.xk_re_size != samples) {
      cerr << "ERROR:" << channel_text << " xk_re_size is incorrect: expected " << samples << ", actual " << outputs.xk_re_size << endl;
      ok = false;
    }
    if (outputs.xk_im_size != samples) {
      cerr << "ERROR:" << channel_text << " xk_im_size is incorrect: expected " << samples << ", actual " << outputs.xk_im_size << endl;
      ok = false;
    }

    // Check xk_re data: only xk_re[0] should be non-zero
    double expected_xk_re_0;
    if (C_HAS_SCALING == 0) {
      expected_xk_re_0 = constant_input * (1 << C_NFFT_MAX);
    } else {
      expected_xk_re_0 = constant_input;
    }
    if (xk_re[0] != expected_xk_re_0) {
      cerr << "ERROR:" << channel_text << " xk_re[0] is incorrect: expected " << expected_xk_re_0 << ", actual " << xk_re[0] << endl;
      ok = false;
    }
    for (i=1; i<samples; i++) {
      if (xk_re[i] != 0.0) {
        cerr << "ERROR:" << channel_text << " xk_re[" << i << "] is incorrect: expected " << 0.0 << ", actual " << xk_re[i] << endl;
        ok = false;
      }
    }

    // Check xk_im data: all values should be zero
    for (i=1; i<samples; i++) {
      if (xk_im[i] != 0.0) {
        cerr << "ERROR:" << channel_text << " xk_im[" << i << "] is incorrect: expected " << 0.0 << ", actual " << xk_im[i] << endl;
        ok = false;
      }
    }

    // Check blk_exp if used: should be nfft
    if (C_HAS_BFP == 1) {
      if (outputs.blk_exp != inputs.nfft) {
        cerr << "ERROR:" << channel_text << " blk_exp is incorrect: expected " << inputs.nfft << ", actual " << outputs.blk_exp << endl;
        ok = false;
      }
    }

    // Check overflow if used: scaling schedule should ensure that overflow never occurs
    if (C_HAS_SCALING == 1 && C_HAS_BFP == 0) {
      if (outputs.overflow != 0) {
        cerr << "ERROR:" << channel_text << " overflow is incorrect: expected " << 0 << ", actual " << outputs.overflow << endl;
        ok = false;
      }
    }

    // That's all of the checks done
    if (ok) {
      cout << "Outputs from simulation" << channel_text << " are correct" << endl;
    } else {
      cout << "Some outputs from simulation" << channel_text << " are incorrect" << endl;
    }

    // Repeat for all channels
    all_ok = all_ok && ok;
  }

  // Destroy the FFT state to free up memory
  xilinx_ip_xfft_v5_0_destroy_state(state);

  // Return value indicates if all outputs of all channels were correct
  if (all_ok) {
    return 0;
  } else {
    return 1;
  }

}
