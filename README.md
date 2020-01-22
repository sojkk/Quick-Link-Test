# Quick-Link-Test
An automatic radio test was developed based on Enhanced Shockburst (ESB), which works for all the nRF Series Chips including, nRF24LE1, nRF24LU1P, nRF51x22, as well as nRF52832. 

## What's it for?
The Purposes of writing this code are:

- Allows R&D engineers to evaluate whether the radio of the device under development works properly in PHY layer- Evaluation mode
- Allows QA engineers to perform PCBA level radio test on predefined channels automatically -Production mode.

## How it works?
- Test the radio link quality more than one frequency points automatically by sending 1000 ESB packets from the tester to the device under test (DUT).

- With ESB, the DUT only sends out ACK to the tester when it receives the correct packets without CRC error.

- By counting the number of ACK received, the tester can determine whether the RF link on a particular frequency points meet the pre-defined PER (packet error ratio) requirement and notify the user thru its MMI (e.g. lights a LED).

- Three frequency points are tested. Testing finished within 5 sec.

- Test Results shown on the Tester side. No need to wire out from DUT

- Small firmware footprint on the DUT (PRX) side

- Program size (bytes) built with Keil uvision 5: Code =3780, RO-data =224, RW-data =152, ZI-data = 2864

##Example code
-The attached demo code works on nRF5 SDK v15.3.0
-Put the folder "quick_link_test" under /examples/proprietary_rf/ to build.