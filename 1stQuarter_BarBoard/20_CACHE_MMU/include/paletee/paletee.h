#ifndef _PALETEE_H_
#define _PALETEE_H_


#define PALETEE_COLOR_WIDTH				(20)
#define PALETEE_TYPE_NUM				(256)
#define RGB888_TRUE_COLOR				(0XFF000000)
#define RGB888_TRUE_MIN					((RGB888_TRUE_COLOR) | 0x00000000)
#define RGB888_TRUE_MAX					((RGB888_TRUE_COLOR) | 0x00FFFFFF)

typedef enum {
	paletee_256_type_Black_SYSTEM,
	paletee_256_type_Maroon_SYSTEM,
	paletee_256_type_Green_SYSTEM,
	paletee_256_type_Olive_SYSTEM,
	paletee_256_type_Navy_SYSTEM,
	paletee_256_type_Purple_SYSTEM,
	paletee_256_type_Teal_SYSTEM,
	paletee_256_type_Silver_SYSTEM,
	paletee_256_type_Grey_SYSTEM,
	paletee_256_type_Red_SYSTEM,
	paletee_256_type_Lime_SYSTEM,
	paletee_256_type_Yellow_SYSTEM,
	paletee_256_type_Blue_SYSTEM,
	paletee_256_type_Fuchsia_SYSTEM,
	paletee_256_type_Aqua_SYSTEM,
	paletee_256_type_White_SYSTEM,
	paletee_256_type_Grey0,
	paletee_256_type_NavyBlue,
	paletee_256_type_DarkBlue,
	paletee_256_type_Blue3,
	paletee_256_type_Blue2,
	paletee_256_type_Blue1,
	paletee_256_type_DarkGreen,
	paletee_256_type_DeepSkyBlue7,
	paletee_256_type_DeepSkyBlue6,
	paletee_256_type_DeepSkyBlue5,
	paletee_256_type_DodgerBlue3,
	paletee_256_type_DodgerBlue2,
	paletee_256_type_Green4,
	paletee_256_type_SpringGreen6,
	paletee_256_type_Turquoise4,
	paletee_256_type_DeepSkyBlue4,
	paletee_256_type_DeepSkyBlue3,
	paletee_256_type_DodgerBlue1,
	paletee_256_type_Green3,
	paletee_256_type_SpringGreen5,
	paletee_256_type_DarkCyan,
	paletee_256_type_LightSeaGreen,
	paletee_256_type_DeepSkyBlue2,
	paletee_256_type_DeepSkyBlue1,
	paletee_256_type_Green2,
	paletee_256_type_SpringGreen4,
	paletee_256_type_SpringGreen3,
	paletee_256_type_Cyan3,
	paletee_256_type_DarkTurquoise,
	paletee_256_type_Turquoise2,
	paletee_256_type_Green1,
	paletee_256_type_SpringGreen2,
	paletee_256_type_SpringGreen1,
	paletee_256_type_MediumSpringGreen,
	paletee_256_type_Cyan2,
	paletee_256_type_Cyan1,
	paletee_256_type_DarkRed2,
	paletee_256_type_DeepPink8,
	paletee_256_type_Purple5,
	paletee_256_type_Purple4,
	paletee_256_type_Purple3,
	paletee_256_type_BlueViolet,
	paletee_256_type_Orange4,
	paletee_256_type_Grey37,
	paletee_256_type_MediumPurple7,
	paletee_256_type_SlateBlue3,
	paletee_256_type_SlateBlue2,
	paletee_256_type_RoyalBlue1,
	paletee_256_type_Chartreuse6,
	paletee_256_type_DarkSeaGreen9,
	paletee_256_type_PaleTurquoise4,
	paletee_256_type_SteelBlue4,
	paletee_256_type_SteelBlue3,
	paletee_256_type_CornflowerBlue,
	paletee_256_type_Chartreuse5,
	paletee_256_type_DarkSeaGreen8,
	paletee_256_type_CadetBlue2,
	paletee_256_type_CadetBlue1,
	paletee_256_type_SkyBlue3,
	paletee_256_type_SteelBlue2,
	paletee_256_type_Chartreuse4,
	paletee_256_type_PaleGreen4,
	paletee_256_type_SeaGreen4,
	paletee_256_type_Aquamarine3,
	paletee_256_type_MediumTurquoise,
	paletee_256_type_SteelBlue1,
	paletee_256_type_Chartreuse3,
	paletee_256_type_SeaGreen3,
	paletee_256_type_SeaGreen2,
	paletee_256_type_SeaGreen1,
	paletee_256_type_Aquamarine2,
	paletee_256_type_DarkSlateGray2,
	paletee_256_type_DarkRed1,
	paletee_256_type_DeepPink7,
	paletee_256_type_DarkMagenta2,
	paletee_256_type_DarkMagenta1,
	paletee_256_type_DarkViolet2,
	paletee_256_type_Purple2,
	paletee_256_type_Orange3,
	paletee_256_type_LightPink4,
	paletee_256_type_Plum4,
	paletee_256_type_MediumPurple6,
	paletee_256_type_MediumPurple5,
	paletee_256_type_SlateBlue1,
	paletee_256_type_Yellow6,
	paletee_256_type_Wheat4,
	paletee_256_type_Grey53,
	paletee_256_type_LightSlateGrey,
	paletee_256_type_MediumPurple4,
	paletee_256_type_LightSlateBlue,
	paletee_256_type_Yellow5,
	paletee_256_type_DarkOliveGreen6,
	paletee_256_type_DarkSeaGreen7,
	paletee_256_type_LightSkyBlue3,
	paletee_256_type_LightSkyBlue2,
	paletee_256_type_SkyBlue2,
	paletee_256_type_Chartreuse2,
	paletee_256_type_DarkOliveGreen5,
	paletee_256_type_PaleGreen3,
	paletee_256_type_DarkSeaGreen6,
	paletee_256_type_DarkSlateGray3,
	paletee_256_type_SkyBlue1,
	paletee_256_type_Chartreuse1,
	paletee_256_type_LightGreen2,
	paletee_256_type_LightGreen1,
	paletee_256_type_PaleGreen2,
	paletee_256_type_Aquamarine1,
	paletee_256_type_DarkSlateGray1,
	paletee_256_type_Red3,
	paletee_256_type_DeepPink6,
	paletee_256_type_MediumVioletRed,
	paletee_256_type_Magenta6,
	paletee_256_type_DarkViolet1,
	paletee_256_type_Purple1,
	paletee_256_type_DarkOrange3,
	paletee_256_type_IndianRed4,
	paletee_256_type_HotPink5,
	paletee_256_type_MediumOrchid4,
	paletee_256_type_MediumOrchid3,
	paletee_256_type_MediumPurple3,
	paletee_256_type_DarkGoldenrod,
	paletee_256_type_LightSalmon3,
	paletee_256_type_RosyBrown,
	paletee_256_type_Grey63,
	paletee_256_type_MediumPurple2,
	paletee_256_type_MediumPurple1,
	paletee_256_type_Gold3,
	paletee_256_type_DarkKhaki,
	paletee_256_type_NavajoWhite3,
	paletee_256_type_Grey69,
	paletee_256_type_LightSteelBlue3,
	paletee_256_type_LightSteelBlue,
	paletee_256_type_Yellow4,
	paletee_256_type_DarkOliveGreen4,
	paletee_256_type_DarkSeaGreen5,
	paletee_256_type_DarkSeaGreen4,
	paletee_256_type_LightCyan3,
	paletee_256_type_LightSkyBlue1,
	paletee_256_type_GreenYellow,
	paletee_256_type_DarkOliveGreen3,
	paletee_256_type_PaleGreen1,
	paletee_256_type_DarkSeaGreen3,
	paletee_256_type_DarkSeaGreen2,
	paletee_256_type_PaleTurquoise1,
	paletee_256_type_Red2,
	paletee_256_type_DeepPink5,
	paletee_256_type_DeepPink4,
	paletee_256_type_Magenta5,
	paletee_256_type_Magenta4,
	paletee_256_type_Magenta3,
	paletee_256_type_DarkOrange2,
	paletee_256_type_IndianRed3,
	paletee_256_type_HotPink4,
	paletee_256_type_HotPink3,
	paletee_256_type_Orchid,
	paletee_256_type_MediumOrchid2,
	paletee_256_type_Orange2,
	paletee_256_type_LightSalmon2,
	paletee_256_type_LightPink3,
	paletee_256_type_Pink3,
	paletee_256_type_Plum3,
	paletee_256_type_Violet,
	paletee_256_type_Gold2,
	paletee_256_type_LightGoldenrod5,
	paletee_256_type_Tan,
	paletee_256_type_MistyRose3,
	paletee_256_type_Thistle3,
	paletee_256_type_Plum2,
	paletee_256_type_Yellow3,
	paletee_256_type_Khaki3,
	paletee_256_type_LightGoldenrod4,
	paletee_256_type_LightYellow3,
	paletee_256_type_Grey84,
	paletee_256_type_LightSteelBlue1,
	paletee_256_type_Yellow2,
	paletee_256_type_DarkOliveGreen2,
	paletee_256_type_DarkOliveGreen1,
	paletee_256_type_DarkSeaGreen1,
	paletee_256_type_Honeydew2,
	paletee_256_type_LightCyan1,
	paletee_256_type_Red1,
	paletee_256_type_DeepPink3,
	paletee_256_type_DeepPink2,
	paletee_256_type_DeepPink1,
	paletee_256_type_Magenta2,
	paletee_256_type_Magenta1,
	paletee_256_type_OrangeRed1,
	paletee_256_type_IndianRed2,
	paletee_256_type_IndianRed1,
	paletee_256_type_HotPink2,
	paletee_256_type_HotPink1,
	paletee_256_type_MediumOrchid1,
	paletee_256_type_DarkOrange1,
	paletee_256_type_Salmon1,
	paletee_256_type_LightCoral,
	paletee_256_type_PaleVioletRed1,
	paletee_256_type_Orchid2,
	paletee_256_type_Orchid1,
	paletee_256_type_Orange1,
	paletee_256_type_SandyBrown,
	paletee_256_type_LightSalmon1,
	paletee_256_type_LightPink1,
	paletee_256_type_Pink1,
	paletee_256_type_Plum1,
	paletee_256_type_Gold1,
	paletee_256_type_LightGoldenrod3,
	paletee_256_type_LightGoldenrod2,
	paletee_256_type_NavajoWhite1,
	paletee_256_type_MistyRose1,
	paletee_256_type_Thistle1,
	paletee_256_type_Yellow1,
	paletee_256_type_LightGoldenrod1,
	paletee_256_type_Khaki1,
	paletee_256_type_Wheat1,
	paletee_256_type_Cornsilk1,
	paletee_256_type_Grey100,
	paletee_256_type_Grey3,
	paletee_256_type_Grey7,
	paletee_256_type_Grey11,
	paletee_256_type_Grey15,
	paletee_256_type_Grey19,
	paletee_256_type_Grey23,
	paletee_256_type_Grey27,
	paletee_256_type_Grey30,
	paletee_256_type_Grey35,
	paletee_256_type_Grey39,
	paletee_256_type_Grey42,
	paletee_256_type_Grey46,
	paletee_256_type_Grey50,
	paletee_256_type_Grey54,
	paletee_256_type_Grey58,
	paletee_256_type_Grey62,
	paletee_256_type_Grey66,
	paletee_256_type_Grey70,
	paletee_256_type_Grey74,
	paletee_256_type_Grey78,
	paletee_256_type_Grey82,
	paletee_256_type_Grey85,
	paletee_256_type_Grey89,
	paletee_256_type_Grey93,
	paletee_256_type_Max,

	/* 256调色板以外的真彩色 */
	paletee_256_typ_ext_true_color = RGB888_TRUE_MIN,
	paletee_256_typ_ext_true_color_max = RGB888_TRUE_MAX
}paletee_256_type_t;

/*
0x800000	maroon 					栗色
0xA52A2A	brown  					褐玫瑰红
0xDC143C	cirson 					暗深红
0x8B0000	darkred					暗红色
0xB22222	firebrick				火砖色		
0xFF0000	red						红色	
	
0xC71585	mediumvioletred			间紫罗兰色	
0xFF1493	deeppink				深粉红色	
0xFF69B4	hotpink					红紫色			
0xFFB6C1	lightpink				亮粉红色	
0xFFF0F5	lavenderblush			淡紫红			
	
	
0xD87093	palevioletred			苍紫罗兰色		
0xFF00FF	fuchsia(magenta)		紫红色		
0xFFC0CB	pink					粉红色		
0xFFE4E1	mistyrose				浅玫瑰色	
	
0x4B0082	indigo					靛青色	
0x8B008B	darkmagenta				暗洋红	
0x8A2BE2	blueviolet		    	紫罗兰色	
0x6A5ACD	slateblue				石蓝色	
0x7B68EE	mediumslateblue	    	间暗蓝色	
0xEE82EE	violet			    	紫罗兰色	
0xD8BFD8	thistle					蓟色		
		
0x800080	purple					紫色			
0x9932CC	darkorchid				暗紫色		
0x9400D3	darkviolet				暗紫罗兰色	
0x9370DB	mediumpurple			间紫色		
0xBA55D3	mediumorchid			间紫色		
0xDDA0DD	plum					洋李色		
0xE6E6FA	lavender				淡紫色		
		
0x8B4513	saddlebrown				重褐色	
0xD2691E	chocolate		    	巧可力色	
0xBC8F8F	rosybrown		    	褐玫瑰红	
0xFA8072	salmon			    	鲜肉色	
0xFF4500	orangered		    	红橙色	
0xFF7F50	coral			    	珊瑚色	
0xF4A460	sandybrown		    	沙褐色	
0xD2B48C	tan				    	茶色（棕褐色）		
0xF5DEB3	wheat			    	浅黄色	
0xFFDEAD	navajowhite		    	纳瓦白	
0xFFE4C4	bisque			    	桔黄色	
0xFFEFD5	papayawhip		    	番木色	
0xFDF5E6	oldlace			    	老花色	
0xFFF5EE	seashell		    	海贝色	
0xFFFAF0	floralwhite		    	花白色	
				
0xA0522D	sienna					赭色		
0xCD5C5C	indianred	        	印第安红	
0xF08080	lightcoral	        	亮珊瑚色	
0xFFA07A	lightsalmon	    		亮肉色	
0xFF6347	tomato		        	西红柿色	
0xFF8C00	darkorange	        	暗桔黄色	
0xCD853F	peru		    		秘鲁色	
0xDEB887	burlywood	    		实木色	
0xFFE4B5	moccasin	   	 		鹿皮色	
0xFFDAB9	peachpuff	        	桃色		
0xFAEBD7	antiquewhite    		古董白	
0xFFF8DC	cornsilk	    		米绸色	
0xFAF0E6	linen		    		亚麻色	
0xFFFAFA	snow		    		雪白色	
0xFFFFF0	ivory		    		象牙色	

0xB8860B	darkgoldenrod			暗金黄色	
0xFFD700	gold					金色		
0xBDB76B	darkkhaki				暗黄褐色	
0xEEE8AA	palegoldenrod			苍麒麟色	
0xFFFACD	lemonchiffon			柠檬绸色	
0xFFFFE0	lightyellow				亮黄色		
						
0xDAA520	goldenrod				金麒麟色	
0xFFFF00	yellow					黄色		
0xF0E68C	khaki					黄褐色		
0xF5F5DC	beige					米色		
0xFAFAD2	lightgoldenrodyellow	亮金黄色	
						
0x2F4F4F	darkslategray			墨绿色		
0x808000	olive					橄榄色		
0x228B22	forestgreen				森林绿		
0x008080	green(teal)				水鸭色		
0x9ACD32	yellowgreen				黄绿色		
0x00FF00	lime					酸橙色		
0x7CFC00	lawngreen				草绿色		
0x00FA9A	mediumspringgreen		间春绿色	
0x90EE90	lightgreen				亮绿色		
0x7FFFD4	aquamarine				碧绿色		
0xF5FFFA	mintcream				薄荷色		
						
0x556B2F	darkolivegreen			暗橄榄绿	
0x006400	darkgreen				暗绿色		
0x2E8B57	seagreen				海绿色		
0x66CDAA	madiumaquamarine		间宝石绿	
0x32CD32	limegreen				橙绿色		
0x7FFF00	chartreuse				黄绿色		
0xADFF2F	greenyellow				黄绿色		
0x00FF7F	springgreen				春绿色		
0x98F898	palegreen				苍绿色		
0xF0FFF0	honeydew				蜜色		
					
0x191970	midnightblue			中灰兰色	
0x00008B	darkblue				暗蓝色		
0x0000CD	mediumblue				间兰色		
0x1E90FF	dodgerblue				闪兰色		
0x00BFFF	deepskyblue				深天蓝色	
0xB0C4DE	lightsteelblue			亮钢兰色	
0x4682B4	steelblue				钢兰色		
0x5F9EA0	cadetblue				军兰色		
0x48D1CC	mediumturquoise			亮绿色		
0x87CECB	skyblue					天蓝色		
0xAFEEEE	paleturquoise			苍宝石绿	
0xF0FFFF	azure					天蓝色		
0x00FFFF	aqua(cyan)				青色		
							
0x000080	navy					海军色		
0x483D8B	darkslateblue			暗灰蓝色	
0x4169E1	royalblue				皇家蓝		
0x6495ED	cornflowerblue			菊兰色		
0x87CEFA	lightskyblue			亮天蓝色	
0xADD8E6	lightblue				亮蓝色		
0x008B8B	darkcyan				暗青色		
0x00CED1	darkturquoise			暗宝石绿	
0x40E0D0	turquoise				青绿色		
0xB0E0E6	powderblue				粉蓝色		
0xE0FFFF	lightcyan				亮青色		
0xF0F8FF	aliceblue				艾利斯兰	
							
0x000000	black					黑色		
0x808000	gray					灰色		
0x778899	lightslategray			亮蓝灰		
0xC0C0C0	silver					银色		
0xDCDCDC	gainsboro				淡灰色		
0xF8F8FF	ghostwhite				幽灵白		
					
0x696969	dimgray					暗灰色		
0x708090	slategray				灰石色		
0xA9A9A9	darkgray				暗灰色		
0xD3D3D3	lightgray				浅灰		
0xF5F5F5	whitesmoke				烟白色		
0xFFFFFF	white					白色		
*/

typedef struct {
	uint32 rgb888;
	uint16 rgb565;
	paletee_256_type_t type;
	char * colorName;
}rgb_256_info_t;

extern void paletee_print_color_select
(
	uint32 x,
	uint32 y,
	paletee_256_type_t color
);
extern void paletee_print_color_point
(
	uint32 x, 
	uint32 y, 
	paletee_256_type_t color
);

#endif	/* _PALETEE_H_ */
