   M S B A M L   `   `   `  ����  Hear2Read_TTS"    Hear2Read_TTS_App.MainWindow   5"   	   R NWindowsBase, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35\ XPresentationFramework, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35W SPresentationCore, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35D 9http://schemas.microsoft.com/winfx/2006/xaml/presentation    5#   	   R NSystem.Xaml, Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089:x,http://schemas.microsoft.com/winfx/2006/xaml     5$   	   @d2http://schemas.microsoft.com/expression/blend/2008     5%   	   Jmc;http://schemas.openxmlformats.org/markup-compatibility/2006     5&   	     � Title$  Hear2Read TTS App��5(   	   $��434.486��6#   $	��700��64    � 
ResizeMode$ CanMinimize=�6@    � Icon$ h2r-32x32-win.ico��5)   	    � 	Resources 5*         SampleTemplate��%2   &	        �� 5,   
   .R�5-      �� 5.      5/       J� InputBindings 51      �� 53   
   
 �� Key$ S��61    �� 	Modifiers$
 Ctrlf�69    �� Command 6   �� *	SaveCmd+69   �� 54   
    �� Gesture$ Ctrl+Shift+S��63    6   �� *	SaveAsCmd+63   �� 55   
   $ Ctrl+O��61    6   �� *	OpenCmd+61   �� 56   
   $ Ctrl+N��60    6   �� *NewCmd+60   �� 57   
   $ Alt+F4��61    6   �� *	ExitCmd+61   .��5:      ]� -   5        $��TopPanel��5:   M   ��. $��0,0,-0.4,-0.2q�6%   $��2��6<   .+�5;   
   �� $	��Top=�6   ���#b3c6ff$��
0,0,-0.0,2q�6?   $��51��6S   .+�5<      ]� ��. $��57��6-   $��	0,0,0.4,0q�69   .+�5=      �� -   5        	 ��Name$
	 Logo��5=      $��Right=�6'   ���#b3c6ff$��0q�6S   .��5>      �� -   5        
 ��Name$
 H2RLogo��5>      $��48��6-   $	��216��69   $	��0 0q�6E   $'��!/Views/hear2readhorizontal@2x.png��6R    J� RenderTransformOrigin$ 0.488,0.329&�6}   5?      �� 5@      $
��Left=�6   ���#b3c6ff$	��22 (�6G   $	��310��6V   $��20,17,20,6.8q�6b   ���	#FF101FBB$ 0.5,0.5&�6�   z�5A      V� 5B      .~�5C      �� �� 5D      �� 5E       �� Angle$ 0.276Q�6.   T� 5F      5G      H2R NVDA Addon Installer5J      �� 5L      $	��Top=�6   $��34��61   $��
-0.4,0,0,0q�6=   5M      5N      �� 5O   
   $	��top=�6   ���#00548b$��40��6?   $��
0,0,-0.4,0q�6K   .+�5P      ]� ��. $��54��6-   $��
0,0,-0.4,0q�69   .+�5Q      �� $
��Left=�6   .+�5R      �� $��20(�6    $
��Wrap=�6.   
�������$��10 10q�6U    �� TextAlignment$
 left=�6d   H2R_Speak NVDA Addon Voices5T      5U      �� 5Y      $��Right=�6   5Z      5[      5\      �� 5]   
   $��Bottom=�6   ���#00548B.+�5^      �� $	��240��6   .��5_      �� -   5         ��Name$ 	VoiceList��5_      $
��Auto=�60   5`      5a      5b      5c      5d      