#ifndef _PALETEE_H_
#define _PALETEE_H_


#define PALETEE_COLOR_WIDTH				(20)
#define PALETEE_16BPP_256COLOR_TYPE_NUM	(256)

typedef enum {
	paletee_16bpp_256color_type_Black_SYSTEM,
	paletee_16bpp_256color_type_Maroon_SYSTEM,
	paletee_16bpp_256color_type_Green_SYSTEM,
	paletee_16bpp_256color_type_Olive_SYSTEM,
	paletee_16bpp_256color_type_Navy_SYSTEM,
	paletee_16bpp_256color_type_Purple_SYSTEM,
	paletee_16bpp_256color_type_Teal_SYSTEM,
	paletee_16bpp_256color_type_Silver_SYSTEM,
	paletee_16bpp_256color_type_Grey_SYSTEM,
	paletee_16bpp_256color_type_Red_SYSTEM,
	paletee_16bpp_256color_type_Lime_SYSTEM,
	paletee_16bpp_256color_type_Yellow_SYSTEM,
	paletee_16bpp_256color_type_Blue_SYSTEM,
	paletee_16bpp_256color_type_Fuchsia_SYSTEM,
	paletee_16bpp_256color_type_Aqua_SYSTEM,
	paletee_16bpp_256color_type_White_SYSTEM,
	paletee_16bpp_256color_type_Grey0,
	paletee_16bpp_256color_type_NavyBlue,
	paletee_16bpp_256color_type_DarkBlue,
	paletee_16bpp_256color_type_Blue3,
	paletee_16bpp_256color_type_Blue2,
	paletee_16bpp_256color_type_Blue1,
	paletee_16bpp_256color_type_DarkGreen,
	paletee_16bpp_256color_type_DeepSkyBlue7,
	paletee_16bpp_256color_type_DeepSkyBlue6,
	paletee_16bpp_256color_type_DeepSkyBlue5,
	paletee_16bpp_256color_type_DodgerBlue3,
	paletee_16bpp_256color_type_DodgerBlue2,
	paletee_16bpp_256color_type_Green4,
	paletee_16bpp_256color_type_SpringGreen6,
	paletee_16bpp_256color_type_Turquoise4,
	paletee_16bpp_256color_type_DeepSkyBlue4,
	paletee_16bpp_256color_type_DeepSkyBlue3,
	paletee_16bpp_256color_type_DodgerBlue1,
	paletee_16bpp_256color_type_Green3,
	paletee_16bpp_256color_type_SpringGreen5,
	paletee_16bpp_256color_type_DarkCyan,
	paletee_16bpp_256color_type_LightSeaGreen,
	paletee_16bpp_256color_type_DeepSkyBlue2,
	paletee_16bpp_256color_type_DeepSkyBlue1,
	paletee_16bpp_256color_type_Green2,
	paletee_16bpp_256color_type_SpringGreen4,
	paletee_16bpp_256color_type_SpringGreen3,
	paletee_16bpp_256color_type_Cyan3,
	paletee_16bpp_256color_type_DarkTurquoise,
	paletee_16bpp_256color_type_Turquoise2,
	paletee_16bpp_256color_type_Green1,
	paletee_16bpp_256color_type_SpringGreen2,
	paletee_16bpp_256color_type_SpringGreen1,
	paletee_16bpp_256color_type_MediumSpringGreen,
	paletee_16bpp_256color_type_Cyan2,
	paletee_16bpp_256color_type_Cyan1,
	paletee_16bpp_256color_type_DarkRed2,
	paletee_16bpp_256color_type_DeepPink8,
	paletee_16bpp_256color_type_Purple5,
	paletee_16bpp_256color_type_Purple4,
	paletee_16bpp_256color_type_Purple3,
	paletee_16bpp_256color_type_BlueViolet,
	paletee_16bpp_256color_type_Orange4,
	paletee_16bpp_256color_type_Grey37,
	paletee_16bpp_256color_type_MediumPurple7,
	paletee_16bpp_256color_type_SlateBlue3,
	paletee_16bpp_256color_type_SlateBlue2,
	paletee_16bpp_256color_type_RoyalBlue1,
	paletee_16bpp_256color_type_Chartreuse6,
	paletee_16bpp_256color_type_DarkSeaGreen9,
	paletee_16bpp_256color_type_PaleTurquoise4,
	paletee_16bpp_256color_type_SteelBlue4,
	paletee_16bpp_256color_type_SteelBlue3,
	paletee_16bpp_256color_type_CornflowerBlue,
	paletee_16bpp_256color_type_Chartreuse5,
	paletee_16bpp_256color_type_DarkSeaGreen8,
	paletee_16bpp_256color_type_CadetBlue2,
	paletee_16bpp_256color_type_CadetBlue1,
	paletee_16bpp_256color_type_SkyBlue3,
	paletee_16bpp_256color_type_SteelBlue2,
	paletee_16bpp_256color_type_Chartreuse4,
	paletee_16bpp_256color_type_PaleGreen4,
	paletee_16bpp_256color_type_SeaGreen4,
	paletee_16bpp_256color_type_Aquamarine3,
	paletee_16bpp_256color_type_MediumTurquoise,
	paletee_16bpp_256color_type_SteelBlue1,
	paletee_16bpp_256color_type_Chartreuse3,
	paletee_16bpp_256color_type_SeaGreen3,
	paletee_16bpp_256color_type_SeaGreen2,
	paletee_16bpp_256color_type_SeaGreen1,
	paletee_16bpp_256color_type_Aquamarine2,
	paletee_16bpp_256color_type_DarkSlateGray2,
	paletee_16bpp_256color_type_DarkRed1,
	paletee_16bpp_256color_type_DeepPink7,
	paletee_16bpp_256color_type_DarkMagenta2,
	paletee_16bpp_256color_type_DarkMagenta1,
	paletee_16bpp_256color_type_DarkViolet2,
	paletee_16bpp_256color_type_Purple2,
	paletee_16bpp_256color_type_Orange3,
	paletee_16bpp_256color_type_LightPink4,
	paletee_16bpp_256color_type_Plum4,
	paletee_16bpp_256color_type_MediumPurple6,
	paletee_16bpp_256color_type_MediumPurple5,
	paletee_16bpp_256color_type_SlateBlue1,
	paletee_16bpp_256color_type_Yellow6,
	paletee_16bpp_256color_type_Wheat4,
	paletee_16bpp_256color_type_Grey53,
	paletee_16bpp_256color_type_LightSlateGrey,
	paletee_16bpp_256color_type_MediumPurple4,
	paletee_16bpp_256color_type_LightSlateBlue,
	paletee_16bpp_256color_type_Yellow5,
	paletee_16bpp_256color_type_DarkOliveGreen6,
	paletee_16bpp_256color_type_DarkSeaGreen7,
	paletee_16bpp_256color_type_LightSkyBlue3,
	paletee_16bpp_256color_type_LightSkyBlue2,
	paletee_16bpp_256color_type_SkyBlue2,
	paletee_16bpp_256color_type_Chartreuse2,
	paletee_16bpp_256color_type_DarkOliveGreen5,
	paletee_16bpp_256color_type_PaleGreen3,
	paletee_16bpp_256color_type_DarkSeaGreen6,
	paletee_16bpp_256color_type_DarkSlateGray3,
	paletee_16bpp_256color_type_SkyBlue1,
	paletee_16bpp_256color_type_Chartreuse1,
	paletee_16bpp_256color_type_LightGreen2,
	paletee_16bpp_256color_type_LightGreen1,
	paletee_16bpp_256color_type_PaleGreen2,
	paletee_16bpp_256color_type_Aquamarine1,
	paletee_16bpp_256color_type_DarkSlateGray1,
	paletee_16bpp_256color_type_Red3,
	paletee_16bpp_256color_type_DeepPink6,
	paletee_16bpp_256color_type_MediumVioletRed,
	paletee_16bpp_256color_type_Magenta6,
	paletee_16bpp_256color_type_DarkViolet1,
	paletee_16bpp_256color_type_Purple1,
	paletee_16bpp_256color_type_DarkOrange3,
	paletee_16bpp_256color_type_IndianRed4,
	paletee_16bpp_256color_type_HotPink5,
	paletee_16bpp_256color_type_MediumOrchid4,
	paletee_16bpp_256color_type_MediumOrchid3,
	paletee_16bpp_256color_type_MediumPurple3,
	paletee_16bpp_256color_type_DarkGoldenrod,
	paletee_16bpp_256color_type_LightSalmon3,
	paletee_16bpp_256color_type_RosyBrown,
	paletee_16bpp_256color_type_Grey63,
	paletee_16bpp_256color_type_MediumPurple2,
	paletee_16bpp_256color_type_MediumPurple1,
	paletee_16bpp_256color_type_Gold3,
	paletee_16bpp_256color_type_DarkKhaki,
	paletee_16bpp_256color_type_NavajoWhite3,
	paletee_16bpp_256color_type_Grey69,
	paletee_16bpp_256color_type_LightSteelBlue3,
	paletee_16bpp_256color_type_LightSteelBlue,
	paletee_16bpp_256color_type_Yellow4,
	paletee_16bpp_256color_type_DarkOliveGreen4,
	paletee_16bpp_256color_type_DarkSeaGreen5,
	paletee_16bpp_256color_type_DarkSeaGreen4,
	paletee_16bpp_256color_type_LightCyan3,
	paletee_16bpp_256color_type_LightSkyBlue1,
	paletee_16bpp_256color_type_GreenYellow,
	paletee_16bpp_256color_type_DarkOliveGreen3,
	paletee_16bpp_256color_type_PaleGreen1,
	paletee_16bpp_256color_type_DarkSeaGreen3,
	paletee_16bpp_256color_type_DarkSeaGreen2,
	paletee_16bpp_256color_type_PaleTurquoise1,
	paletee_16bpp_256color_type_Red2,
	paletee_16bpp_256color_type_DeepPink5,
	paletee_16bpp_256color_type_DeepPink4,
	paletee_16bpp_256color_type_Magenta5,
	paletee_16bpp_256color_type_Magenta4,
	paletee_16bpp_256color_type_Magenta3,
	paletee_16bpp_256color_type_DarkOrange2,
	paletee_16bpp_256color_type_IndianRed3,
	paletee_16bpp_256color_type_HotPink4,
	paletee_16bpp_256color_type_HotPink3,
	paletee_16bpp_256color_type_Orchid,
	paletee_16bpp_256color_type_MediumOrchid2,
	paletee_16bpp_256color_type_Orange2,
	paletee_16bpp_256color_type_LightSalmon2,
	paletee_16bpp_256color_type_LightPink3,
	paletee_16bpp_256color_type_Pink3,
	paletee_16bpp_256color_type_Plum3,
	paletee_16bpp_256color_type_Violet,
	paletee_16bpp_256color_type_Gold2,
	paletee_16bpp_256color_type_LightGoldenrod5,
	paletee_16bpp_256color_type_Tan,
	paletee_16bpp_256color_type_MistyRose3,
	paletee_16bpp_256color_type_Thistle3,
	paletee_16bpp_256color_type_Plum2,
	paletee_16bpp_256color_type_Yellow3,
	paletee_16bpp_256color_type_Khaki3,
	paletee_16bpp_256color_type_LightGoldenrod4,
	paletee_16bpp_256color_type_LightYellow3,
	paletee_16bpp_256color_type_Grey84,
	paletee_16bpp_256color_type_LightSteelBlue1,
	paletee_16bpp_256color_type_Yellow2,
	paletee_16bpp_256color_type_DarkOliveGreen2,
	paletee_16bpp_256color_type_DarkOliveGreen1,
	paletee_16bpp_256color_type_DarkSeaGreen1,
	paletee_16bpp_256color_type_Honeydew2,
	paletee_16bpp_256color_type_LightCyan1,
	paletee_16bpp_256color_type_Red1,
	paletee_16bpp_256color_type_DeepPink3,
	paletee_16bpp_256color_type_DeepPink2,
	paletee_16bpp_256color_type_DeepPink1,
	paletee_16bpp_256color_type_Magenta2,
	paletee_16bpp_256color_type_Magenta1,
	paletee_16bpp_256color_type_OrangeRed1,
	paletee_16bpp_256color_type_IndianRed2,
	paletee_16bpp_256color_type_IndianRed1,
	paletee_16bpp_256color_type_HotPink2,
	paletee_16bpp_256color_type_HotPink1,
	paletee_16bpp_256color_type_MediumOrchid1,
	paletee_16bpp_256color_type_DarkOrange1,
	paletee_16bpp_256color_type_Salmon1,
	paletee_16bpp_256color_type_LightCoral,
	paletee_16bpp_256color_type_PaleVioletRed1,
	paletee_16bpp_256color_type_Orchid2,
	paletee_16bpp_256color_type_Orchid1,
	paletee_16bpp_256color_type_Orange1,
	paletee_16bpp_256color_type_SandyBrown,
	paletee_16bpp_256color_type_LightSalmon1,
	paletee_16bpp_256color_type_LightPink1,
	paletee_16bpp_256color_type_Pink1,
	paletee_16bpp_256color_type_Plum1,
	paletee_16bpp_256color_type_Gold1,
	paletee_16bpp_256color_type_LightGoldenrod3,
	paletee_16bpp_256color_type_LightGoldenrod2,
	paletee_16bpp_256color_type_NavajoWhite1,
	paletee_16bpp_256color_type_MistyRose1,
	paletee_16bpp_256color_type_Thistle1,
	paletee_16bpp_256color_type_Yellow1,
	paletee_16bpp_256color_type_LightGoldenrod1,
	paletee_16bpp_256color_type_Khaki1,
	paletee_16bpp_256color_type_Wheat1,
	paletee_16bpp_256color_type_Cornsilk1,
	paletee_16bpp_256color_type_Grey100,
	paletee_16bpp_256color_type_Grey3,
	paletee_16bpp_256color_type_Grey7,
	paletee_16bpp_256color_type_Grey11,
	paletee_16bpp_256color_type_Grey15,
	paletee_16bpp_256color_type_Grey19,
	paletee_16bpp_256color_type_Grey23,
	paletee_16bpp_256color_type_Grey27,
	paletee_16bpp_256color_type_Grey30,
	paletee_16bpp_256color_type_Grey35,
	paletee_16bpp_256color_type_Grey39,
	paletee_16bpp_256color_type_Grey42,
	paletee_16bpp_256color_type_Grey46,
	paletee_16bpp_256color_type_Grey50,
	paletee_16bpp_256color_type_Grey54,
	paletee_16bpp_256color_type_Grey58,
	paletee_16bpp_256color_type_Grey62,
	paletee_16bpp_256color_type_Grey66,
	paletee_16bpp_256color_type_Grey70,
	paletee_16bpp_256color_type_Grey74,
	paletee_16bpp_256color_type_Grey78,
	paletee_16bpp_256color_type_Grey82,
	paletee_16bpp_256color_type_Grey85,
	paletee_16bpp_256color_type_Grey89,
	paletee_16bpp_256color_type_Grey93,
	paletee_16bpp_256color_type_Max
}paletee_16bpp_256color_type;

#if 0
#define PALETEE_16BPP_Black_SYSTEM 			(0x0000)
#define PALETEE_16BPP_Maroon_SYSTEM         (0x8000)
#define PALETEE_16BPP_Green_SYSTEM          (0x0400)
#define PALETEE_16BPP_Olive_SYSTEM          (0x8400)
#define PALETEE_16BPP_Navy_SYSTEM           (0x0010)
#define PALETEE_16BPP_Purple_SYSTEM         (0x8010)
#define PALETEE_16BPP_Teal_SYSTEM           (0x0410)
#define PALETEE_16BPP_Silver_SYSTEM         (0x8410)
#define PALETEE_16BPP_Grey_SYSTEM           (0x8410)
#define PALETEE_16BPP_Red_SYSTEM            (0xf800)
#define PALETEE_16BPP_Lime_SYSTEM           (0x07e0)
#define PALETEE_16BPP_Yellow_SYSTEM         (0xffe0)
#define PALETEE_16BPP_Blue_SYSTEM           (0x001f)
#define PALETEE_16BPP_Fuchsia_SYSTEM        (0xf81f)
#define PALETEE_16BPP_Aqua_SYSTEM           (0x07ff)
#define PALETEE_16BPP_White_SYSTEM          (0xffff)
#define PALETEE_16BPP_Grey0                 ()
#define PALETEE_16BPP_NavyBlue              ()
#define PALETEE_16BPP_DarkBlue              ()
#define PALETEE_16BPP_Blue3                 ()
#define PALETEE_16BPP_Blue2                 ()
#define PALETEE_16BPP_Blue1                 ()
#define PALETEE_16BPP_DarkGreen             ()
#define PALETEE_16BPP_DeepSkyBlue7          ()
#define PALETEE_16BPP_DeepSkyBlue6          ()
#define PALETEE_16BPP_DeepSkyBlue5          ()
#define PALETEE_16BPP_DodgerBlue3           ()
#define PALETEE_16BPP_DodgerBlue2           ()
#define PALETEE_16BPP_Green4                ()
#define PALETEE_16BPP_SpringGreen6          ()
#define PALETEE_16BPP_Turquoise4            ()
#define PALETEE_16BPP_DeepSkyBlue4          ()
#define PALETEE_16BPP_DeepSkyBlue3          ()
#define PALETEE_16BPP_DodgerBlue1           ()
#define PALETEE_16BPP_Green3                ()
#define PALETEE_16BPP_SpringGreen5          ()
#define PALETEE_16BPP_DarkCyan              ()
#define PALETEE_16BPP_LightSeaGreen         ()
#define PALETEE_16BPP_DeepSkyBlue2          ()
#define PALETEE_16BPP_DeepSkyBlue1          ()
#define PALETEE_16BPP_Green2                ()
#define PALETEE_16BPP_SpringGreen4          ()
#define PALETEE_16BPP_SpringGreen3          ()
#define PALETEE_16BPP_Cyan3                 ()
#define PALETEE_16BPP_DarkTurquoise         ()
#define PALETEE_16BPP_Turquoise2            ()
#define PALETEE_16BPP_Green1                ()
#define PALETEE_16BPP_SpringGreen2          ()
#define PALETEE_16BPP_SpringGreen1          ()
#define PALETEE_16BPP_MediumSpringGreen     ()
#define PALETEE_16BPP_Cyan2                 ()
#define PALETEE_16BPP_Cyan1                 ()
#define PALETEE_16BPP_DarkRed2              ()
#define PALETEE_16BPP_DeepPink8             ()
#define PALETEE_16BPP_Purple5               ()
#define PALETEE_16BPP_Purple4               ()
#define PALETEE_16BPP_Purple3               ()
#define PALETEE_16BPP_BlueViolet            ()
#define PALETEE_16BPP_Orange4               ()
#define PALETEE_16BPP_Grey37                ()
#define PALETEE_16BPP_MediumPurple7         ()
#define PALETEE_16BPP_SlateBlue3            ()
#define PALETEE_16BPP_SlateBlue2            ()
#define PALETEE_16BPP_RoyalBlue1            ()
#define PALETEE_16BPP_Chartreuse6           ()
#define PALETEE_16BPP_DarkSeaGreen9         ()
#define PALETEE_16BPP_PaleTurquoise4        ()
#define PALETEE_16BPP_SteelBlue4            ()
#define PALETEE_16BPP_SteelBlue3            ()
#define PALETEE_16BPP_CornflowerBlue        ()
#define PALETEE_16BPP_Chartreuse5           ()
#define PALETEE_16BPP_DarkSeaGreen8         ()
#define PALETEE_16BPP_CadetBlue2            ()
#define PALETEE_16BPP_CadetBlue1            ()
#define PALETEE_16BPP_SkyBlue3              ()
#define PALETEE_16BPP_SteelBlue2            ()
#define PALETEE_16BPP_Chartreuse4           ()
#define PALETEE_16BPP_PaleGreen4            ()
#define PALETEE_16BPP_SeaGreen4             ()
#define PALETEE_16BPP_Aquamarine3           ()
#define PALETEE_16BPP_MediumTurquoise       ()
#define PALETEE_16BPP_SteelBlue1            ()
#define PALETEE_16BPP_Chartreuse3           ()
#define PALETEE_16BPP_SeaGreen3             ()
#define PALETEE_16BPP_SeaGreen2             ()
#define PALETEE_16BPP_SeaGreen1             ()
#define PALETEE_16BPP_Aquamarine2           ()
#define PALETEE_16BPP_DarkSlateGray2        ()
#define PALETEE_16BPP_DarkRed1              ()
#define PALETEE_16BPP_DeepPink7             ()
#define PALETEE_16BPP_DarkMagenta2          ()
#define PALETEE_16BPP_DarkMagenta1          ()
#define PALETEE_16BPP_DarkViolet2           ()
#define PALETEE_16BPP_Purple2               ()
#define PALETEE_16BPP_Orange3               ()
#define PALETEE_16BPP_LightPink4            ()
#define PALETEE_16BPP_Plum4                 ()
#define PALETEE_16BPP_MediumPurple6         ()
#define PALETEE_16BPP_MediumPurple5         ()
#define PALETEE_16BPP_SlateBlue1            ()
#define PALETEE_16BPP_Yellow6               ()
#define PALETEE_16BPP_Wheat4                ()
#define PALETEE_16BPP_Grey53                ()
#define PALETEE_16BPP_LightSlateGrey        ()
#define PALETEE_16BPP_MediumPurple4         ()
#define PALETEE_16BPP_LightSlateBlue        ()
#define PALETEE_16BPP_Yellow5               ()
#define PALETEE_16BPP_DarkOliveGreen6       ()
#define PALETEE_16BPP_DarkSeaGreen7         ()
#define PALETEE_16BPP_LightSkyBlue3         ()
#define PALETEE_16BPP_LightSkyBlue2         ()
#define PALETEE_16BPP_SkyBlue2              ()
#define PALETEE_16BPP_Chartreuse2           ()
#define PALETEE_16BPP_DarkOliveGreen5       ()
#define PALETEE_16BPP_PaleGreen3            ()
#define PALETEE_16BPP_DarkSeaGreen6         ()
#define PALETEE_16BPP_DarkSlateGray3        ()
#define PALETEE_16BPP_SkyBlue1              ()
#define PALETEE_16BPP_Chartreuse1           ()
#define PALETEE_16BPP_LightGreen2           ()
#define PALETEE_16BPP_LightGreen1           ()
#define PALETEE_16BPP_PaleGreen2            ()
#define PALETEE_16BPP_Aquamarine1           ()
#define PALETEE_16BPP_DarkSlateGray1        ()
#define PALETEE_16BPP_Red3                  ()
#define PALETEE_16BPP_DeepPink6             ()
#define PALETEE_16BPP_MediumVioletRed       ()
#define PALETEE_16BPP_Magenta6              ()
#define PALETEE_16BPP_DarkViolet1           ()
#define PALETEE_16BPP_Purple1               ()
#define PALETEE_16BPP_DarkOrange3           ()
#define PALETEE_16BPP_IndianRed4            ()
#define PALETEE_16BPP_HotPink5              ()
#define PALETEE_16BPP_MediumOrchid4         ()
#define PALETEE_16BPP_MediumOrchid3         ()
#define PALETEE_16BPP_MediumPurple3         ()
#define PALETEE_16BPP_DarkGoldenrod         ()
#define PALETEE_16BPP_LightSalmon3          ()
#define PALETEE_16BPP_RosyBrown             ()
#define PALETEE_16BPP_Grey63                ()
#define PALETEE_16BPP_MediumPurple2         ()
#define PALETEE_16BPP_MediumPurple1         ()
#define PALETEE_16BPP_Gold3                 ()
#define PALETEE_16BPP_DarkKhaki             ()
#define PALETEE_16BPP_NavajoWhite3          ()
#define PALETEE_16BPP_Grey69                ()
#define PALETEE_16BPP_LightSteelBlue3       ()
#define PALETEE_16BPP_LightSteelBlue        ()
#define PALETEE_16BPP_Yellow4               ()
#define PALETEE_16BPP_DarkOliveGreen4       ()
#define PALETEE_16BPP_DarkSeaGreen5         ()
#define PALETEE_16BPP_DarkSeaGreen4         ()
#define PALETEE_16BPP_LightCyan3            ()
#define PALETEE_16BPP_LightSkyBlue1         ()
#define PALETEE_16BPP_GreenYellow           ()
#define PALETEE_16BPP_DarkOliveGreen3       ()
#define PALETEE_16BPP_PaleGreen1            ()
#define PALETEE_16BPP_DarkSeaGreen3         ()
#define PALETEE_16BPP_DarkSeaGreen2         ()
#define PALETEE_16BPP_PaleTurquoise1        ()
#define PALETEE_16BPP_Red2                  ()
#define PALETEE_16BPP_DeepPink5             ()
#define PALETEE_16BPP_DeepPink4             ()
#define PALETEE_16BPP_Magenta5              ()
#define PALETEE_16BPP_Magenta4              ()
#define PALETEE_16BPP_Magenta3              ()
#define PALETEE_16BPP_DarkOrange2           ()
#define PALETEE_16BPP_IndianRed3            ()
#define PALETEE_16BPP_HotPink4              ()
#define PALETEE_16BPP_HotPink3              ()
#define PALETEE_16BPP_Orchid                ()
#define PALETEE_16BPP_MediumOrchid2         ()
#define PALETEE_16BPP_Orange2               ()
#define PALETEE_16BPP_LightSalmon2          ()
#define PALETEE_16BPP_LightPink3            ()
#define PALETEE_16BPP_Pink3                 ()
#define PALETEE_16BPP_Plum3                 ()
#define PALETEE_16BPP_Violet                ()
#define PALETEE_16BPP_Gold2                 ()
#define PALETEE_16BPP_LightGoldenrod5       ()
#define PALETEE_16BPP_Tan                   ()
#define PALETEE_16BPP_MistyRose3            ()
#define PALETEE_16BPP_Thistle3              ()
#define PALETEE_16BPP_Plum2                 ()
#define PALETEE_16BPP_Yellow3               ()
#define PALETEE_16BPP_Khaki3                ()
#define PALETEE_16BPP_LightGoldenrod4       ()
#define PALETEE_16BPP_LightYellow3          ()
#define PALETEE_16BPP_Grey84                ()
#define PALETEE_16BPP_LightSteelBlue1       ()
#define PALETEE_16BPP_Yellow2               ()
#define PALETEE_16BPP_DarkOliveGreen2       ()
#define PALETEE_16BPP_DarkOliveGreen1       ()
#define PALETEE_16BPP_DarkSeaGreen1         ()
#define PALETEE_16BPP_Honeydew2             ()
#define PALETEE_16BPP_LightCyan1            ()
#define PALETEE_16BPP_Red1                  ()
#define PALETEE_16BPP_DeepPink3             ()
#define PALETEE_16BPP_DeepPink2             ()
#define PALETEE_16BPP_DeepPink1             ()
#define PALETEE_16BPP_Magenta2              ()
#define PALETEE_16BPP_Magenta1              ()
#define PALETEE_16BPP_OrangeRed1            ()
#define PALETEE_16BPP_IndianRed2            ()
#define PALETEE_16BPP_IndianRed1            ()
#define PALETEE_16BPP_HotPink2              ()
#define PALETEE_16BPP_HotPink1              ()
#define PALETEE_16BPP_MediumOrchid1         ()
#define PALETEE_16BPP_DarkOrange1           ()
#define PALETEE_16BPP_Salmon1               ()
#define PALETEE_16BPP_LightCoral            ()
#define PALETEE_16BPP_PaleVioletRed1        ()
#define PALETEE_16BPP_Orchid2               ()
#define PALETEE_16BPP_Orchid1               ()
#define PALETEE_16BPP_Orange1               ()
#define PALETEE_16BPP_SandyBrown            ()
#define PALETEE_16BPP_LightSalmon1          ()
#define PALETEE_16BPP_LightPink1            ()
#define PALETEE_16BPP_Pink1                 ()
#define PALETEE_16BPP_Plum1                 ()
#define PALETEE_16BPP_Gold1                 ()
#define PALETEE_16BPP_LightGoldenrod3       ()
#define PALETEE_16BPP_LightGoldenrod2       ()
#define PALETEE_16BPP_NavajoWhite1          ()
#define PALETEE_16BPP_MistyRose1            ()
#define PALETEE_16BPP_Thistle1              ()
#define PALETEE_16BPP_Yellow1               ()
#define PALETEE_16BPP_LightGoldenrod1       ()
#define PALETEE_16BPP_Khaki1                ()
#define PALETEE_16BPP_Wheat1                ()
#define PALETEE_16BPP_Cornsilk1             ()
#define PALETEE_16BPP_Grey100               ()
#define PALETEE_16BPP_Grey3                 ()
#define PALETEE_16BPP_Grey7                 ()
#define PALETEE_16BPP_Grey11                ()
#define PALETEE_16BPP_Grey15                ()
#define PALETEE_16BPP_Grey19                ()
#define PALETEE_16BPP_Grey23                ()
#define PALETEE_16BPP_Grey27                ()
#define PALETEE_16BPP_Grey30                ()
#define PALETEE_16BPP_Grey35                ()
#define PALETEE_16BPP_Grey39                ()
#define PALETEE_16BPP_Grey42                ()
#define PALETEE_16BPP_Grey46                ()
#define PALETEE_16BPP_Grey50                ()
#define PALETEE_16BPP_Grey54                ()
#define PALETEE_16BPP_Grey58                ()
#define PALETEE_16BPP_Grey62                ()
#define PALETEE_16BPP_Grey66                ()
#define PALETEE_16BPP_Grey70                ()
#define PALETEE_16BPP_Grey74                ()
#define PALETEE_16BPP_Grey78                ()
#define PALETEE_16BPP_Grey82                ()
#define PALETEE_16BPP_Grey85                ()
#define PALETEE_16BPP_Grey89                ()
#define PALETEE_16BPP_Grey93               	()
#endif
extern void paletee_print_color_select(uint32 x, uint32 y, uint8 color);
extern void paletee_print_color_point
(
	uint32 x, 
	uint32 y, 
	paletee_16bpp_256color_type color
);

#endif	/* _PALETEE_H_ */
