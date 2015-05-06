###Homemade GSM Network (OpenBTS and USRP B100)
> ITS MAGIC ITS MAGIC I CAN MAKE A PHONE CALL FOR FREE

#### OpenBTS and Linux Management
*Secrets of the lost CLI*
* To start the system
    1. Turn it on, and press F9 to select the "ubuntu" option under UEFI
    2. Log in as "btsuser", password: "userpass"
    3. Open two terminals and split-screen them (or tabs, just get two terminals side by side, or easily accessible simultaneously)
    4. In one, change into the /OpenBTS directory and launch OpenBTSCLI
    5. In the other, gain root (su - ) password: Supercap123 and change into /root/lib/
    6. You are now ready to go, all 6 lights should be lit on the USRP B100

* Adding New Clients
    1. Change into the $Netman directory
    2. In your OpenBTS CLI, get the current LUR attempts


* Tips and Tricks / Troubleshooting
    * its finicky
    * get used to reloading the stack (asterisk, sipauthserver, openbts, smqueue)

#### USRP B100
*What is this off-white box anyways?*
* Cabling
    * VERT900 Antennas on RF1 and RF2
    * Ensure they are positioned for minimal signal interference (90° is typically workable)
    * USB Cable goes into computer (plug before booting)
    * Power goes into wall (plug this in before booting computer, ensure 4 of front lights are on)

* What is it?
    * FPGA (Field Programmable Gate Array
    * SDR (Software Defined Radio)
    * 4 Antenna Headers (2 RF, 2 something else)
    * We use VERT900 Antennas to work with the GSM bands (850, 900, 1800, 1900)

* What is it not?
    * magic
    * the answer to all life's problems
    * your key to free cell service.
