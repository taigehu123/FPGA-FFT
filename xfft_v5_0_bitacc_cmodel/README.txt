                                       

                    Core Name: Xilinx FFT v5.0 C model
                    Version: 5.0
                    Release Date: October 10, 2007


================================================================================

This document contains the following sections: 

1. Introduction
2. New Features
3. Resolved Issues
4. Known Issues 
5. Technical Support
6. Other Information (optional)
7. Model Release History
 
================================================================================
 
1. INTRODUCTION

For the most recent updates to the IP installation instructions for this model,
please go to:

   http://www.xilinx.com/ipcenter/coregen/ip_update_install_instructions.htm

 
For system requirements:

   http://www.xilinx.com/ipcenter/coregen/ip_update_system_requirements.htm 



This file contains release notes for the Xilinx LogiCORE FFT v5.0 C model
solution. For the latest core and model updates, see the product page at:
 
  www.xilinx.com/xlnx/xebiz/designResources/ip_product_details.jsp?key=FFT.


2. NEW FEATURES  
 
   - First release of this C model.

 
3. RESOLVED ISSUES 
 
   - n/a
                                             
 
4. KNOWN ISSUES 
   
   The following are known issues for v5.0 of this core / model at time of release:

   - Error in FFT v5.0 datasheet: C_CHANNELS is not a generic used for the C model
      - Table 4 on page 16 of the FFT v5.0 datasheet states that C_CHANNELS is a C model generic
      - This is an error in the datasheet: C_CHANNELS is not used for the C model
      - The C model is a single channel model only
      - To model a multichannel FFT, see the C model documentation
      - CR451474

  The most recent information, including known issues, workarounds, and
  resolutions for this version is provided in the release notes Answer Record
  for the ISE 9.2i IP Update at 

     http://www.xilinx.com/xlnx/xil_ans_display.jsp?getPagePath=29209


5. TECHNICAL SUPPORT 

   To obtain technical support, create a WebCase at www.xilinx.com/support.
   Questions are routed to a team with expertise using this product.  
     
   Xilinx provides technical support for use of this product when used
   according to the guidelines described in the core documentation, and
   cannot guarantee timing, functionality, or support of this product for
   designs that do not follow specified guidelines.


6. OTHER INFORMATION
   
   Full documentation of the C model is provided in the document
   "xfft_v5_0_bitacc_cmodel.pdf".


7. MODEL RELEASE HISTORY 

Date        By            Version      Description
================================================================================
10/10/2007  Xilinx, Inc.  5.0          First release of C model, for FFT v5.0
================================================================================


(c) 2007 Xilinx, Inc. All Rights Reserved.

XILINX, the Xilinx logo, and other designated brands included herein are
trademarks of Xilinx, Inc. All other trademarks are the property of their
respective owners.

Xilinx is disclosing this user guide, manual, release note, and/or
specification (the Documentation) to you solely for use in the development
of designs to operate with Xilinx hardware devices. You may not reproduce, 
distribute, republish, download, display, post, or transmit the Documentation
in any form or by any means including, but not limited to, electronic,
mechanical, photocopying, recording, or otherwise, without the prior written 
consent of Xilinx. Xilinx expressly disclaims any liability arising out of
your use of the Documentation.  Xilinx reserves the right, at its sole 
discretion, to change the Documentation without notice at any time. Xilinx
assumes no obligation to correct any errors contained in the Documentation, or
to advise you of any corrections or updates. Xilinx expressly disclaims any
liability in connection with technical support or assistance that may be
provided to you in connection with the information. THE DOCUMENTATION IS
DISCLOSED TO YOU AS-IS WITH NO WARRANTY OF ANY KIND. XILINX MAKES NO 
OTHER WARRANTIES, WHETHER EXPRESS, IMPLIED, OR STATUTORY, REGARDING THE
DOCUMENTATION, INCLUDING ANY WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
PARTICULAR PURPOSE, OR NONINFRINGEMENT OF THIRD-PARTY RIGHTS. IN NO EVENT 
WILL XILINX BE LIABLE FOR ANY CONSEQUENTIAL, INDIRECT, EXEMPLARY, SPECIAL, OR
INCIDENTAL DAMAGES, INCLUDING ANY LOSS OF DATA OR LOST PROFITS, ARISING FROM
YOUR USE OF THE DOCUMENTATION.
