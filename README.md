System-Tray-Program
===================

This is an adapted work of a person named Matthew Ellis. As no license was provided, I'm releasing this as GPL. If anyone has any objections to it, please let me know immediately. A valid request for ownership would be accepted only from m.t.ellis at bigfoot.com, as this is the email address provided in the code.

The program at hand, is a neat app that runs in the system tray of Windows. I adapted it to display a personalised message to the user at fixed intervals, asking the user to take a break from using the computer.

Where to make changes:

In MimimizeDemo.cpp:
Change REST_INTERVAL_IN_SECOND to vary the time and scroll down to find and change MessageBox(NULL, "Time to rest your eyes Navin", "Hey!", MB_ICONINFORMATION|MB_SETFOREGROUND);

