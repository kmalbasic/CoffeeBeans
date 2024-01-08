# CoffeeBeans
Dynamic-link library (with a twist) that prevents sleep and makes you look like a hardworking employee!

# Why?
This little project was initially made out of boredom to prove how shit security is within a company
valued 2.34 billion USD. And I hate when I have to log back into company software because it times out every
one and a half minutes.

(_this was used in gamehacking back in 2017 and ArmA 2, who would've thought it's useful in 2023_)

# How can I use this with my fully locked, monitored workstation?
This is where this project really shines when it comes to stupidity of IT security of my company at least. Since 
Windows 7 or Windows Server 2008, Microsoft added a snug little feature that will inject any DLL (well, depends if
you have Secure boot enabled **I don't, 2.34 billion dollars cough**) into all processes that are currently running.
It's called **AppInit_DLLs**.

# What is AppInit_DLLs and how does it bypass all the antiviruses and execution policy protection measures?
**_AppInit_DLLs_** is a feature that can be toggled on any device running Windows 7 or higher by simply altering 
the registry in this specific pattern:

**X64**
```
[HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Windows]
	"LoadAppInit_DLLs"=dword:00000001                        -- We set LoadAppInit_DLLs value to 1 to enable this feature
	"AppInit_DLLs"="c:\\Users\\M\\Desktop\\windows64.dll"    -- We set the path to our fine DLL that will do whatever we need
	"RequireSignedAppInit_DLLs"=dword:00000000               -- We set a parameter that will allow us to load unsigned DLLs (only if Secure Boot is disabled)
```

**X86 (32-bit)** _descriptions of registry keys are same for both x64/x86_
```
[HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Windows NT\CurrentVersion\Windows]
	"LoadAppInit_DLLs"=dword:00000001
	"AppInit_DLLs"="c:\\Users\\M\\Desktop\\windows.dll"
	"RequireSignedAppInit_DLLs"=dword:00000000
```

When we are done configuring and editing our registry keys, we just need our DLL and we are off to the tracks!

# Things to pay attention to when using AppInit_DLLs

- When creating your DLL, please be sure to check what executable it's currently injected into to avoid unneccessary performance drops and
  continuous code execution. Keep it running within a single executable, what the fuck are you, Skynet?
- AppInit_DLLs is loading the DLL with LoadAppInitDlls() from Kernel32.dll which is called by User32.dll during it's initialisation (older versions
  call it directly from User32.dll), this can be detected as malicious if you DLL is running malicious code (not CoffeeBeans) and is not digitally signed.
- Be sure to either obfuscate your DLL or sign it with a leaked certificate before running code that is not presented like mine (harmless and just won't trigger
  any AV patterns)
- Before actually commiting to testing, test your library with a DLL injector and your process, you don't want to inject your DLL into everything and expect
  things to go smoothly first thing.
- **DO NOT** execute commands that are not meant to be executed during runtime (AppInit_DLLs also injects into processes before the Windows GUI loads),
  a very nice **SYSTEM_SERVICE_EXCEPTION** BSOD will appear!
- This was tested on Windows 10 22H2 (build 19045.3633) and Windows 11 22H2 (build 22000.2538). Will test on older systems within the VM and update this if I am ever bored enough.

# Words of wisdom and honor.

RIP **caffeine64/caffeine32** and fuck your endpoint security with your execution policies. I booted Linux off of your machine and prevented all your kernel modules from loading. 
Yes, administrator rights with no logging or monitoring. Full freedom.
