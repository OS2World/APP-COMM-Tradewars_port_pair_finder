             README for Tradewars 2002 port pair finder v0.07


                  Contents ..........................  1

               I. Legal Stuff .......................  2

              II. Overview ..........................  2
    
             III. Getting Sector Data ...............  2

              IV. Running the Program ...............  3

               V. Output ............................  6

              VI. Limitations .......................  6
 
             VII. Performance .......................  6

            VIII. Notes on Program Behavior .........  7

              IX. How it Works ......................  7
                                                       
               X. Compiling the Program .............  7 

              XI. Author Information ................  8

             XII. History ...........................  8





























                                   Page 1
I. Legal Stuff

This program is provided free of charge, and I'm not liable for any damage 
it may do, nor do I guarantee any level of functionality.  You know the 
drill.

The 32-bit DPMI executable is compiled with a GNU compiler, so it's 
probably subject to the GNU public license, though I'm not particularly 
inclined to wade through the documentation to find out the details.

If you make any modifications, I require that you make an effort to contact 
me before distributing the modified form.  

================================================================================

II. Overview

If you don't play the BBS online door game Tradewars 2002, then you'll find 
this particular program quite useless indeed.

If you do play, however, you may still find it useless, as there are helper 
programs, and other utilities, which do the same thing.

Most helpers, however, don't support 20,000 sectors, nor do the utilities.

This will be most useful to people who either don't use helpers, or don't 
use them often, and have scripts for trading or stealing/robbing activity.

What this program does is read in sector and port information from CIM 
captures, and figure out where the adjacent ports are, of the type you 
specify on the command line.

You can specify multiple types on the same command line, and all matching 
port pairs will be found.  Duplicate listings will not be made, should 
there be pairs which match more than one type.

The types of port pairs currently supported:

   BBS - SSB  - Triple trading
   BxS - SxB  - Equipment/Fuel Ore trading 
   xBS - xSB  - Equipment/Organics trading
   BBS - BSB  - Equipment/Organics trading, buying Fuel Ore
   BSx - SBx  - Organics/Fuel Ore trading
   xxB - xxB  - Steal/Sell/Move
   xxS - xxS  - Rob/Move

Adding other types would be trivial, but I can't think of any others that 
are particularly useful.

================================================================================

III. Getting Sector and Port Data

The way this program gets the sector and port information is from a file 
you create by capturing the computer CIM output in the game.  


                                   Page 2
How you do this will depend on the communications program you use.

TW Helper has an option to create TWVIEW and TWASSIST files, which for each 
includes the CIM output.  Execute the /V command to write the *.SCT and 
*.PRT files that contain sector and port data.  These files can be used as 
is by the program.

If you're not using TW Helper, you can always get the information by 
creating capture files in your communications program.  In the game, go to 
the ship computer, and enter the following sequence, using your numeric 
keypad:

ALT-200
ALT-201
ALT-202
ALT-203
ALT-204
ALT-205
ALT-206

That will put you into Computer Interrogation Mode, with a plain looking 
":" for a prompt.  At this point, turn on your sector information capture, 
and press "I".

After the sector information has finished scrolling, end the capture.  This 
capture file will be what you point the program to for sector data.

Then turn on your port information capture, and press "R".

After the port information has finished scrolling, end the capture.  This 
capture file will be what you point the program to for port data.

The program is tolerant of junk in the file, but makes one important 
assumption:  if the line begins with a valid sector number, it considers 
the line full of pertinent data.  If it isn't, junk on that line will 
cause an abort.  Other than that, lines which begin with non-numeric data 
are ignored.

================================================================================

IV. Running the Program

Once you've exported the sector and port data from the game, you're ready 
to run the program. 

The options are available by simply running the program without parameters, 
or with /h or /?, and are as follows:

/n# - number of sectors in universe (100 to 65535); default 1000
/c# - minimum capacity of relevant products (0 to 65535); default 0
/a - use capacities as reported - no calculations
/p# - priority class (1 to 4); default=2
/d# - priority delta (-31 to 31); default=0
/z# - zap port in sector
/b<filename> - binary port data file
/s<filename> - sector data file

                                   Page 3
/r<filename> - port data file
/o<filename> - report file; default 'pairs.lst'
/t<mask> - type of ports to find:

           BBS_SSB - Triple-trading pairs
           BxS_SxB - Equipment/Fuel Ore trading
           xBS_xSB - Equipment/Organics trading
           BBS_BSB - Equipment/Organics trading, buying Fuel Ore
           BSx_SBx - Organics/Fuel Ore trading
           xxB_xxB - Sell/Steal/Move
           xxS_xxS - Rob/Move

Detailed explanation of options:

Option /n#:  This specifies the number of sectors in the universe.  Here is 
where the amount of storage set aside for sector information is determined.  
If the CIM file contains information for a sector number greater than this 
value, the program will abort.  The maximum value that the DOS version will 
support is 5460.  The OS/2 and DPMI versions support up to 65535 sectors
(currently, TW Gold supports only 20000 sectors).  The default value is 
1000 sectors.

Option /c#:  This specifies the minimum capacity of relevant products for 
the port to be considered part of a pair.  For triple-trading pairs, all 
products are relevant.  For equipment/organics trading, buying fuel ore, 
organics and equipment are the relevant products.  For all other types, the 
relevant products are those not represented by an 'x' in the mask.

Option /a:  This tells the program not to calculate full capacities based 
on reported capacities and percentages, but to use the reported capacities 
as is.  Using this with a recent port report will allow you to selectively 
exclude drained ports with the /c parameter.

Option /p#:  This is specific to the OS/2 version, and specifies the 
priority class that the program runs in while searching for port pairs.  
Allowable values are integers from 1 to 4, where 1 denotes idle-class, 2 
denotes standard class, 3 denotes time-critical class, and 4 denotes 
foreground-server class.  The order of precedence, from lowest priority to 
highest, is 1-2-4-3.  This is only put into effect after the sector 
information has been loaded, and the bubble search has begun.  The idea 
here was to be able to run the program at idle priority in the background 
for a long search (for many port pairs), while doing other things.  If 
you specify a value of 3, which is the highest priority, then a timeslice 
will be forfeited every 100 sectors, to allow you the chance to abort it 
(otherwise, keyboard input won't be processed in a timely fashion).  The 
result is that the program will run somewhat slower at time-critical 
priority than otherwise, unless there are other high-priority programs that 
are being unfriendly to the CPU.  The default value is 2.

Option /d#:  This is specific to the OS/2 version, and specifies the 
priority delta value.  This is a subdivision of the priority class, with 
valid values from -31 to 31.  Giving a value below 0 will put the priority 
slightly below other programs of the same class which have no delta, 
thereby being more friendly without going down an entire class.  Giving a 
value above 0 will put the priority slightly above other such programs, 
making this program run somewhat faster if other CPU-intensive tasks are 

                                   Page 4
running.  The default value is 0.

Option /z#:  This specifies that a port no longer exists (or is 
inaccessible), so should be removed from the binary port data file.  
This option may appear on the command line as many times as will fit in the 
limitations of the maximum command length.  If this isn't used, ports which 
no longer exist will continue to be reported in pairs, as long as the given 
binary data file is used.

Option /b<filename>:  This specifies the binary data file where port 
information is stored.  This is how you keep track of ports that no longer 
show up in the CIM report due to deployed fighters.  The capacities stored 
are those last reported in the CIM, whether actual or computed maximum.
This file will be read if it exists, and created if it doesn't, after the 
CIM data is read in.

Option /s<filename>:  This specifies the name of the file which contains 
the CIM sector data capture.  There is no default, so this option must be 
used.

Option /r<filename>:  This specifies the name of the file which contains 
the CIM port data capture.  There is no default, so this option must be 
used.

Option /o<filename>:  This specifies the name of the file which the found 
port pairs will be written to.  The specified file will be overwritten if it 
exists.  You may also use anything which the operating system can handle as 
a file, such as a named pipe, communications port, network printer name, 
etc.  The default is "pairs.lst".

Option /t<mask>:  This specifies a type of port pair to search for.  The 
types of pairs possible are:

           BBS_SSB - Triple-trading pairs
           BxS_SxB - Equipment/Fuel Ore trading
           xBS_xSB - Equipment/Organics trading
           BBS_BSB - Equipment/Organics trading, buying Fuel Ore
           BSx_SBx - Organics/Fuel Ore trading
           xxB_xxB - Sell/Steal/Move
           xxS_xxS - Rob/Move

The mask is not case sensitive, and more than one mask may be provided on 
the same command line.  If no masks are provided, then xBS_xSB is assumed, 
as it is the most commonly sought port pair.  The 'x' in the mask means it 
doesn't matter whether the port buys or sells in that area, for the 
purposes of that type of port pair. 

There is no space between the switch and the option data, so assuming a 
sector data file of "sectors.txt", you might do the following:

twppfind.exe /ssectors.txt /n5000 /rports.txt /tbsx_sbx /txxb_xxb

This would read sector data from 'sectors.txt', port dat afrom 'ports.txt', 
for a 5000-sector universe, then search for port pairs that allow 
organics/fuel ore cross-trading or sell/steal/move operations.  The output 
is written to the default of 'pairs.lst'.

                                   Page 5

=============================================================================

V. Output

Since all output is accomplished via stream I/O, you can use anything that 
the underlying operating system can open as a file for output.  This 
includes I/O devices, such as LPT1, CON, etc.

The format of the output is this:

 1278 - SBB -  2080  1540  2280          1318 - SBB -  1607  2370  2000
15921 - SBB -  2410  1360   970         18717 - SBB -  2060  2390  1990

 1349 - BBB -  2210  1020   810          1351 - SBB -  2500  1270  1910
 4883 - SBB -  2080  1640  1020          3345 - BSB -  1330  2980  1720

The pairs above are 1278-15921, 1318-18717, 1349-4883, and 1351-3345.

Next to the sector number is the port type, followed by the capacities for 
fuel ore, organics, and equipment, respectively.

=============================================================================

VI. Limitations

The program was written to accommodate up to 65,535 sectors, since the Gold 
version of TW currently allows up to 20,000.  In the unlikely event that the 
game ever exceeds 65,535 sectors, it would not be very difficult to change 
datatypes to long integers to increase capacity.  

The 16-bit DOS version is limited something between 5000 and 10000 sectors, 
depending on the available memory, average number of warps per sector, and 
port density of the universe.  For some reason, the Borland executable 
crashes with an attempt at allocating too much memory for the sector array.  
With OS/2, this meant a locked DOS session that needed to be killed.  On a 
PC-DOS machine, the result was a spontaneous reboot.  This occurred at 
around 15,800 or so sectors, so make sure to stay below 15,000 regardless 
(the program won't handle that anyway).  It should reliably handle any 
5000-sector universe, however.

The DPMI and OS/2 executables will handle the program design limits.  As 
soon as I get Linux installed, I'll build an executable for that as well, 
which will of course not have any memory problems, either.

=============================================================================

VII. Performance

I make no pretensions about my skill as a programmer.  I'm strictly a 
novice, but I'd like to think that the program is fairly efficient, for 
what it's doing.  

The program is processor-bound, so your memory and disk subsystems are 
insignificant.  Your video subsystem may also have an impact, especially if 
running in a windowed session of whatever operating system you're using.  

                                   Page 6
Time the execution with output redirected to nul, and you'll see the 
difference.

On my Pentium II/266MHz system, looking for equipment/organics 
cross-trading pairs in a completely explored 5000-sector universe, the 
program takes about 3 seconds from start to finish.  The DOS version will 
be slower than the OS/2 and DPMI versions.

The more types of ports to find, the longer the program will take to run, 
of course.

================================================================================

VIII. Notes on Program Behavior

1)  The reported port capacity is the total capacity, not just what's in 
the report.  If the port is at less than 100%, the value given is divided 
by the percentage, itself divided by 100.  If the value given is greater 
than 0, and the percentage is reported as 0 (a recently depleted port), the 
percentage is assumed to be 1%; thus, the capacity reported may not be 
terribly accurate.  Even with non-zero percentages, the calculations are 
limited by the fact that the percentage has only two significant figures.
This does not apply when using the /a option, to suppress capacity 
calculations.

2)  If the capacity is reported to be 0, that means the value given in the 
port report was 0, so determining the actual capacity is impossible.  It 
may also mean that the port in question is under construction, and has no 
capacity yet.

3)  At any time during the actual port search, you may hit the space bar
to abort, and save the pairs found up to that point.

=============================================================================

IX. How it Works

Here's the basic logic of how it works:

1)  For each sector, provided it has a port, determine if that port is the 
first half of any of the desired port pair types.  If so, continue on.

2)  Loop through the outwarps of the current sector, and if they are not 
one-way, and the adjacent sector has a port, determine if it's the second 
half of the type(s) of port pairs already half-matched by the first sector.  
If so, add it to the results, if it's not already in there (if it matched a 
previous port pair type as well).

That's it.  Really simple.

================================================================================

X. Compiling the Program

The source for the program is included in the 'source.zip' archive.


                                   Page 7
The language is C++, and I've tried to make the program cross-platform, 
though it may or may not be necessary to make some small modifications 
before it will compile for you. 

There are a few defines in place that were necessary to make it compile 
under the compilers I've been using.  These may or may not need to be 
removed or altered to allow compilation on other compilers, for other 
platforms.

There's nothing sneaky going on, so just compile each .cpp file, and link 
all the objects together as one executable.  I keep separate sources for 
ease of editing.  There are makefiles for all the compilers that I used 
included.

The four included executables are:

twppos2.exe  - 32-bit OS/2 VIO executable 
twppdpmi.exe - 32-bit DPMI executable
twppdos.exe  - 16-bit DOS executable (8086 instruction set)

The first was compiled with IBM VisualAge C++ 3.08 for OS/2, the second 
with the DJGPP package (a DOS port of GCC), and the third with an old copy 
of Borland C++ 3.0 for DOS.

=============================================================================

XI. Author Information

The "I" all through the rest of this file refers to me, Mike Ruskai, and I 
can be reached via e-mail to thannymeister@yahoo.com, should you have a 
question, suggestion, or problem report.

=============================================================================

XII. History

01-09-99   Initial release of version 0.01.

01-11-99   Changed port data collection completely, to accomodate the fact
           that the dash to indicate that the port buys can be part of the
           same word as the capacity, when it's 10000 or greater.  

           Added overflow protection, in case a rounding error makes the
           calculated port capacity greater than the max of an unsigned
           short integer.

           Added port type BBS - BSB, which restricts equipment/organics
           pairs to ones that must buy fuel ore.  This type of pair is 
           easier to use a trading script with, since no prompts to buy
           fuel ore need to be skipped, allowing the scripts to be dumber.

           Altered the storage of sector data to give the DOS version a 
           higher sector capacity, of something like 10000.  The Borland
           executable crashes when overallocating the sector array, 
           however, which I have no explanation for.  Running out of memory
           during operation, however, produces a graceful failure.  Perhaps 

                                   Page 8
           I can get a hold of a better 16-bit DOS compiler.

           Version 0.02 here.

01-16-99   Added two new options:

                /c# - minimum capacity of relevant products
                /a  - use capacities as is, no calculations

           The first specifies what a ports minimum capacity must be in
           the relevant product areas (for that pair type), and the second
           dictates that port capacities should be used as is, with no
           calculations to determine full capacities.

           Version 0.03.

01-29-99   Made some slight performance improvements by using memcpy() 
           instead of iterative loops.

           Next on the list is the addition of a database, to report on
           ports obscured by fighters, or destroyed.

           This is version 0.04.

03-11-99   Added the ability to store binary port information in a file 
           specified on the command line.  This allows the program to keep 
           track of ports which no longer show up in the CIM report because 
           of deployed fighters.  The type of the port along with its 
           capacities are stored.  The capacities will be the last ones
           reported in the CIM data, whether actual or computed maximums.

           The binary data is read first, so that CIM data will override 
           anything in the binary file.

           Added the ability to zap ports from the binary data file, so 
           that destroyed or permanently inaccessible ports won't be 
           reported as part of a pair.  This zapping is done after both the 
           binary port data and CIM port data is read in, so you can 
           exclude any sectors even if they're accessible.

           This is version 0.05.

03-14-99   Neglected to open the binary file with the ios::binary flag, so 
           there may have been some spurious data here and there.  Also
           change the writing to use a character instead of the default
           enumeration variable size for the port type, so that the data
           is 7 bytes per port.  

           Fixed the duplicate port checking.  I had switched the order of 
           port loading at the last second, so that the CIM reading 
           function would think port information read from the binary file 
           was an indication of duplicate CIM data.  Changed the order back
           to the way it was, since the binary port read doesn't override
           the existent port information for the sector anyway.

           Changed the output to put an asterik to the left of any sectors

                                   Page 9
           whose ports were read from the binary file, but not the CIM 
           file.  This, of course, means sectors with an asterik indicate
           either deployed fighters, or a destroyed port.

           This is version 0.06.

08-17-99   No functionality changes.  Just updating the e-mail address.

           Version 0.07.
















































                                   Page 11
