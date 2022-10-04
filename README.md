<h1 align="center">The Kurumi Source Engine, explorer Engine</h1>

<img src="https://github.com/keowu/testesreadme/blob/main/pics/1.png?raw=true">

<h3 align="center">Simplified description of the project objective</h3>

> A CS Source Engine² for creating cheat debug valve codes, exploring hidden game engine features, exploit development, assembly analyzer or a full network emulator, a custom scripting interface for study and cloud-based decompiler for testing (and binary analysis), think about it maybe as a basis for you to make your own tool, or use it however you want, after all it exists for studies.

> ! This project was created just for portfolio so I don't guarantee there will be updates and I can discontinue whenever I want.

<img src="https://github.com/keowu/testesreadme/blob/main/pics/2.png?raw=true" width="6912" height="400">

<h3 align="center">Project features</h3>

- Network emulator
- Intercept encrypted logs and network traffic
- Intercept game and developer logs and hidden logs(Vac, Feedback and game crash/stuck)
- Draw on the game's own interface with their own API
- A custom injector based on syscall (know what you are doing first)
- Send your own network packets
- Modify network packages
- Store logs of captured or emulator packages for private analysis
- Use all processor cores for processing
- Own scripting systems that are easy to understand and learn
- Possibility of Disassembler by Address, Pattern, All .text section or all binary sections.
- Possibility of using a cloud-based decompiler provided by me for my own tests.
- Possibility to create patches in specific regions based on patterns or addresses using the scripting system.
- Possibility to remove and restore patch in specific regions applied.
- Possibility to disable or enable patches
- Possibility to download patches from other shared users to use
- Other functions that I want to apply or that someone contributes or improves.

<img src="https://github.com/keowu/testesreadme/blob/main/pics/3.png?raw=true" width="6912" height="400">

<h3 align="center">Scripting system</h3>

> The scripting language used was nicknamed by a friend of Kurumi Lang, with it it is possible to customize several tasks that the application must perform, new features may appear in the future, so I will introduce you a little about this scripting language

<p>Let's assume you want to get the main executable from memory, hl2.exe, there are several ways to do this, so that you don't have to use external ways you can use the following script:</p>

```
  IMGBASE("hl2.exe")
```

<p>With this, the base of the image will be automatically returned in memory, but let's suppose now that I had my IDA open and out of nowhere I found an instruction that checks the RVA 0x1234, I don't need to keep adding the address there is a much more process easy thanks to the scripting system, but before this use we use the concatenation symbol that tells the script that it must join both values, for example:</p>

```
  IMGBASE("hl2.exe")+RVA("0x1234")
```

<p>The "+" symbol has the function of joining both addresses, and the RVA function will be used to retrieve that address and at the end we can obtain the relative address that contains that desired instruction, the prefix "0x" is not mandatory more help in identification. There is another instruction that also has a similar function, we can often work with compressed or virtualized or obfuscated binaries, and they do not always have a default address or RVA offset, in this case we use the memory byte patterns, the engine also supports you You can see an example of usage below:</p>

```
  IMGBASE("hl2.exe")+PATTERN("40??90??10")
```

<p>That way the engine will know to look for the pattern in the mapped sections of the binary.</p>

<p>The scripting language is very important new features may arrive soon, and these features are used in two places you can check the screenshot below:</p>

<div align="center">
<img src="https://github.com/keowu/testesreadme/blob/main/pics/b1.png?raw=true">
</div>

<p>Sometimes you can choose the options more precisely what action you want to do, for example:</p>

<div align="center">
<img src="https://github.com/keowu/testesreadme/blob/main/pics/b2.png?raw=true">
</div>

<p>Another use for the scripting language is for the patching system, here's a quick example:</p>

<div align="center">
<img src="https://github.com/keowu/testesreadme/blob/main/pics/b3.png?raw=true">
</div>

<p>After we insert the script we are asked to insert the shellcode we want, in this next example I put two "nop" instructions, standard from the intel manual.</p>

<div align="center">
<img src="https://github.com/keowu/testesreadme/blob/main/pics/b4.png?raw=true">
</div>

<p>And after that, our engine prepared, activated the patch and left it on a list so that we can modify our game:</p>

<div align="center">
<img src="https://github.com/keowu/testesreadme/blob/main/pics/b5.png?raw=true">
</div>

<p>The scripting system is very useful and will definitely speed up your patch life and organization.</p>

<div align="center">
<img src="https://github.com/keowu/testesreadme/blob/main/pics/4.png?raw=true" width="6912" height="400">
</div>

<h3 align="center">Patching List</h3>

> With the patch list system you can make several quick modifications, it is possible to create a new patch in a certain address or region of the binary or search by memory pattern, when created this patch is automatically activated and added to the list and from there you You can remove, disable or enable whenever you want.

<p>Let's see more didactic usage examples:</p>

<div align="center">
<img src="https://github.com/keowu/testesreadme/blob/main/pics/b6.png?raw=true">
</div>

<p>In this example, on the 1st click a prompt will open informing you to insert the script for the engine, 2nd I inform this script, in this case I informed that it should look for the module "server.dll" loaded and direct to the RVA "0x122266" and there put a shellcode when clicking ok, notice that in 3rd the tree is normal, and that in the disassembler we can see the instructions responsible for aligning the leaf with the trunk of the tree.</p>

<div align="center">
<img src="https://github.com/keowu/testesreadme/blob/main/pics/b7.png?raw=true">
</div>

<p>1st Notice that the window requesting the shellcode was opened after pressing OK, I informed that it would be 0x90, 0x90, "referencing two nop instructions", basically the processor will ignore the execution and go straight, pay attention to the 3rd one which is the trunk of the tree aligned with its leaf.</p>

<div align="center">
<img src="https://github.com/keowu/testesreadme/blob/main/pics/b8.png?raw=true">
</div>

<p>Notice that in 1st a patch was added to the list, and the game was applied, see in 2nd that the instructions have already been modified in Disassembler View, 3rd you can see that we can already see the difference in the game and that the trees are no longer aligned with their change, 4th you can disable this patch or enable it whenever you want, and finally on the "-" button you disable the patch and recover all the instructions of the modified region that are stored as backup in a model class.</p>

<img src="https://github.com/keowu/testesreadme/blob/main/pics/5.png?raw=true" width="6912" height="400">

<h3 align="center">Disassembler Explorer</h3>

<div align="center">
<img src="https://github.com/keowu/testesreadme/blob/main/pics/b9.png?raw=true">
</div>

<p>In the 1st you must inform the script you want to run, define a custom search option, and finally in the 2nd you will get all the Disassembler output after completion by the engine.</p>

<div align="center">
<img src="https://github.com/keowu/testesreadme/blob/main/pics/6.png?raw=true" width="6912" height="400">
</div>

<h3 align="center">Cloud Based Pseudo-C Decompiler</h3>

> I'm working on my own compiler studying open source projects, currently I need a lot of binaries to study and improve, that's when the idea to create this function in this project came up, it's still under development but the technical details are as follows:

<p>When a binary is sent to the server for decompilation, the content is encrypted with a private key shared between the user and the server, the server receives all the data and allocates a position in the queue for the user to wait until the binary is decompiled on the server. server and returns an index of the queue, this index is checked every period of time until the server indicates a payload of the content of the decompiler encrypting with the shared key that only the user has access, thus making the decompiled code available.</p>

<div align="center">
<img src="https://github.com/keowu/testesreadme/blob/main/pics/7.png?raw=true" width="6912" height="400">
</div>

<h3 align="center">Demo Videos</h3>

> Here you can check out videos I recorded, showing how the project works and its features, I hope you like it.


[![Watch the video](https://i3.ytimg.com/vi/iRUhYC6w2pI/hqdefault.jpg)](https://youtu.be/iRUhYC6w2pI)


<div align="center">
<img src="https://github.com/keowu/testesreadme/blob/main/pics/8.png?raw=true" width="6912" height="400">
</div>

<h3 align="center">Content Download</h3>

> Here you can download all my IDB's and my complete analysis see my organization chart while I was developing my project, I hope this helps you to learn or share more knowledge and improve the project.

<h4 align="center"><a href="https://mega.nz/folder/HLIBmBJS#SSYLMX7L2HmRNWbw_JlwVw" target="_blank">Complete shared folder</a></h4>

<h4>File list: </h4>

- <a href="https://mega.nz/file/qCxDzZrQ#JPhYi0wcAOHNTnTskBizNsgj1WKQy2wvVXnrsQ8NzwI" target="_blank">client.dll.idb (IDA 7.5)</a>
- <a href="https://mega.nz/file/mSQkWDzC#oTcDxX5hEa-FV5xwZ1Y0gJzORrFlOm7p1g42or7ei2c" target="_blank">engine.dll.idb (IDA 7.5)</a>
- <a href="https://mega.nz/file/7SZ3nTRA#TUJfAD6HqXPK9Jg1MLly2JTkjv8Ic8hvkYMC-qmp7Ko" target="_blank">server.dll.idb (IDA 7.5)</a>
- <a href="https://mega.nz/file/iGRxHQhA#J1u8dxsHmzHhDOnaEEQfFj56m58N4F60HZ7PLRhm0fA" target="_blank">steamclient.dll_after_21_09_2022.idb (IDA 7.5)</a>
- <a href="https://mega.nz/file/rXQURDjT#c8gup8pUWe5iNXePp_QZN47XFMTqsjxtL3A43HMN-nY" target="_blank">steamclient.dll_before_21_09_2022.idb (IDA 7.5)</a>
- <a href="https://mega.nz/file/iSwjCQpR#R1Q63bNcapZyPL8IQiM583v4wLKnqZP7O843UZ8CIGc" target="_blank">tier0.dll.idb (IDA 7.5)</a>

<h5>Have good studies or good contributions to the community and to the project :)</h5>

<div align="center">
<img src="https://github.com/keowu/testesreadme/blob/main/pics/9.png?raw=true" width="6912" height="400">
</div>

<h3 align="center">Contribuite/Future</h3>

> You can contribute to the project by suggesting features or improvements.

<p>Any like, issue, PR's are welcome, as long as they are polite and good for the long-term development of the project, as it is a testing project.</p>

<h6>Special thanks to the developers of <a href="https://github.com/BeaEngine/beaengine">BeaEngine</a>, for providing a fast, stable and very useful engine</h6>
<h6>Many thanks to <a href="https://github.com/AkkoS2">"AkkoS2"</a>, for contributing suggesting content and editing the videos images and managing the security of the repository.</h6>
