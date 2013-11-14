Craft is an Othello program with relatively strong artificial intelligence. It uses techniques including pattern-based evaluation model, bit-board representation, Multi-Prob Cut, NegaScout, iterative-widening End-Cut and self-learning opening book. Craft can look forward 16~20 steps in mid-game, and up to 24 steps perfect endgame.

Craft's AI has now reached almost the same level as the famous Othello program '伤心黑白棋'.

==================

How to build Craft

1    Check out Craft source.

2    Open "Othello\Craft.sln" using Microsoft Visual Studio 2012.

3    Right click "Craft Engine" in the solution explorer, and choose "Properties".

4    Expand to "Configuration\Linker\Optimization". Make sure "Link time code generation" option has been set to "/ltcg" for the "Release" configurations you want to build(Win32 and x64).

5    Build the project.

6    To run Craft, copy the contents under "Othello\Game" folder into where the executable is built. For example, "Release|Win32" -> "Othello\Release", "Release|x64" -> "Othello\x64\Release". 

Notes

Since the offical release is built using Profile-Guided Optimizations(PGO), your own build may not be as fast as the official one. If you want to create your own PGO build, follow the instructions at http://msdn.microsoft.com/en-us/library/e7k32f4k(VS.80).aspx .
