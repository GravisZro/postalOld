QT=

#SmartHeap Stuff.  Normally, we define these in debug mode and comment them out otherwise.
#DEFINES += "NOSHMALLOC"
#DEFINES += "MEM_DEBUG"
#DEFINES += "DEFINE_NEW_MACRO"

#Define this for debug mode, comment it out otherwise.
#DEFINES += "_DEBUG"

DEFINES += "BUILD_CHEAT"
DEFINES += "BUILD_CHEAT_NETWORK"
DEFINES += "PLATFORM_UNIX"

DEFINES += "US=1"
DEFINES += "UK=2"
DEFINES += "GERMAN=3"
DEFINES += "FRENCH=4"
DEFINES += "JAPAN=5"
DEFINES += "LOCALE=US"

#DEFINES += "APP_NAME=\"\\\"Postal Plus MP\\\"\""
#DEFINES += "MAIN_MENU_TITLE=\"\\\"POSTAL PLUS MP\\\"\""


#DEFINES += "APP_NAME=\"\\\"Postal Plus Demo\\\"\""
#DEFINES += "MAIN_MENU_TITLE=\"\\\"POSTAL PLUS DEMO\\\"\""


#DEFINES += "APP_NAME=\"\\\"Postal Plus\\\"\""
#DEFINES += "MAIN_MENU_TITLE=\"\\\"POSTAL PLUS\\\"\""

DEFINES += "APP_NAME=\"\\\"Postal\\\"\""
DEFINES += "MAIN_MENU_TITLE=\"\\\"POSTAL\\\"\""

DEFINES += "PREFS_FILE=\"\\\"Postal.ini\\\"\""

DEFINES += "ALLOW_JOYSTICK"

DEFINES += "MULTIPLAYER_REMOVED"
DEFINES += "EDITOR_REMOVED"
DEFINES += "LOADLEVEL_REMOVED"

INCLUDEPATH += Toolkit/RSPiX
INCLUDEPATH += Toolkit/WishPiX

LIBS += -lSDL2

HEADERS += \
    PostalAttrib.h \
    Network/ProtoBSDIP.h \
    Reality.h \
    Realm.h \
    SampleMaster.h \
    Scene.h \
    Scoreboard.h \
    Settings.h \
    Smash.h \
    Network/Socket.h \
    Sprites.h \
    StockPile.h \
    TexEdit.h \
    Thing.h \
    Title.h \
    Toolbar.h \
    TriggerRegions.h \
    TriggerRgn.h \
    Update.h \
    Alpha.h \
    AlphaAnimType.h \
    AlphaBlit.h \
    Animated3D.h \
    Average.h \
    BufQ.h \
    Bulletfest.h \
    Camera.h \
    Collision.h \
    Crawler.h \
    Credits.h \
    CtrlBuf.h \
    Cutscene.h \
    Encrypt.h \
    Game.h \
    GameSettings.h \
    GameTime.h \
    Grip.h \
    IdBank.h \
    Input.h \
    InputSettings.h \
    InputSettingsDlg.h \
    Keys.h \
    Localize.h \
    Log.h \
    LogTab.h \
    main.h \
    MemFileFest.h \
    Menus.h \
    MenuSettings.h \
    MenuTrans.h \
    Message.h \
    Network/Net.h \
    Organ.h \
    Personatorium.h \
    Play.h \
    Thing/AnimatedThing.h \
    Thing/Ball.h \
    Thing/Bouy.h \
    Thing/Chunk.h \
    Thing/Demon.h \
    Thing/Dispenser.h \
    Thing/Explode.h \
    Thing/Fire.h \
    Thing/GameEdit.h \
    Thing/GoalTimer.h \
    Thing/Hood.h \
    Thing/NavigationNet.h \
    Thing/Pylon.h \
    Thing/SoundRelay.h \
    Thing/SoundThing.h \
    Thing/Thing3D.h \
    Thing/Trigger.h \
    Thing/Warp.h \
    Thing/Weapon.h \
    Thing/Weapon/DeathWad.h \
    Thing/Weapon/Fireball.h \
    Thing/Weapon/Firebomb.h \
    Thing/Weapon/Grenade.h \
    Thing/Weapon/Heatseeker.h \
    Thing/Weapon/Mine.h \
    Thing/Weapon/Napalm.h \
    Thing/Weapon/Rocket.h \
    Thing/Thing3D/Barrel.h \
    Thing/Thing3D/Character.h \
    Thing/Thing3D/Flag.h \
    Thing/Thing3D/FlagBase.h \
    Thing/Thing3D/Item3D.h \
    Thing/Thing3D/Item3D/PowerUp.h \
    Thing/Thing3D/Character/Doofus.h \
    Thing/Thing3D/Character/Dude.h \
    Thing/Thing3D/Character/Doofus/Band.h \
    Thing/Thing3D/Character/Doofus/Ostrich.h \
    Thing/Thing3D/Character/Doofus/Person.h \
    Thing/Thing3D/Character/Doofus/Sentry.h \
    Toolkit/RSPiX/BLUE/unix/UnixBlue.h \
    Toolkit/RSPiX/BLUE/unix/UnixBlueKeys.h \
    Toolkit/RSPiX/BLUE/unix/UnixSystem.h \
    Toolkit/RSPiX/BLUE/Blue.h \
    Toolkit/RSPiX/BLUE/System.h \
    Toolkit/RSPiX/CYAN/Unix/UnixCyan.h \
    Toolkit/RSPiX/CYAN/cyan.h \
    Toolkit/RSPiX/GREEN/3D/pipeline.h \
    Toolkit/RSPiX/GREEN/3D/render.h \
    Toolkit/RSPiX/GREEN/3D/types3d.h \
    Toolkit/RSPiX/GREEN/3D/user3d.h \
    Toolkit/RSPiX/GREEN/3D/zbuffer.h \
    Toolkit/RSPiX/GREEN/BLiT/RPrint.h \
    Toolkit/RSPiX/GREEN/Image/Image.h \
    Toolkit/RSPiX/GREEN/Image/ImageFile.h \
    Toolkit/RSPiX/GREEN/Image/PalFile.h \
    Toolkit/RSPiX/GREEN/Image/SpecialTyp.h \
    Toolkit/RSPiX/GREEN/InputEvent/InputEvent.h \
    Toolkit/RSPiX/GREEN/Mix/mix.h \
    Toolkit/RSPiX/GREEN/Mix/MixBuf.h \
    Toolkit/RSPiX/GREEN/Sample/sample.h \
    Toolkit/RSPiX/GREEN/Snd/snd.h \
    Toolkit/RSPiX/GREEN/SndFx/SndFx.h \
    Toolkit/RSPiX/GREEN/Task/task.h \
    Toolkit/RSPiX/ORANGE/Attribute/attribute.h \
    Toolkit/RSPiX/ORANGE/CDT/advqueue.h \
    Toolkit/RSPiX/ORANGE/CDT/flist.h \
    Toolkit/RSPiX/ORANGE/CDT/fqueue.h \
    Toolkit/RSPiX/ORANGE/CDT/List.h \
    Toolkit/RSPiX/ORANGE/CDT/listbase.h \
    Toolkit/RSPiX/ORANGE/CDT/pixel.h \
    Toolkit/RSPiX/ORANGE/CDT/PQueue.h \
    Toolkit/RSPiX/ORANGE/CDT/slist.h \
    Toolkit/RSPiX/ORANGE/CDT/smrtarry.h \
    Toolkit/RSPiX/ORANGE/CDT/stack.h \
    Toolkit/RSPiX/ORANGE/Channel/channel.h \
    Toolkit/RSPiX/ORANGE/Chips/chip.h \
    Toolkit/RSPiX/ORANGE/color/colormatch.h \
    Toolkit/RSPiX/ORANGE/color/dithermatch.h \
    Toolkit/RSPiX/ORANGE/Debug/profile.h \
    Toolkit/RSPiX/ORANGE/DirtRect/DirtRect.h \
    Toolkit/RSPiX/ORANGE/File/file.h \
    Toolkit/RSPiX/ORANGE/GameLib/Region.h \
    Toolkit/RSPiX/ORANGE/GameLib/Shapes.h \
    Toolkit/RSPiX/ORANGE/GUI/btn.h \
    Toolkit/RSPiX/ORANGE/GUI/dlg.h \
    Toolkit/RSPiX/ORANGE/GUI/edit.h \
    Toolkit/RSPiX/ORANGE/GUI/Frame.h \
    Toolkit/RSPiX/ORANGE/GUI/guiItem.h \
    Toolkit/RSPiX/ORANGE/GUI/ListBox.h \
    Toolkit/RSPiX/ORANGE/GUI/ListContents.h \
    Toolkit/RSPiX/ORANGE/GUI/MultiBtn.h \
    Toolkit/RSPiX/ORANGE/GUI/ProcessGui.h \
    Toolkit/RSPiX/ORANGE/GUI/PushBtn.h \
    Toolkit/RSPiX/ORANGE/GUI/scrollbar.h \
    Toolkit/RSPiX/ORANGE/GUI/txt.h \
    Toolkit/RSPiX/ORANGE/IFF/iff.h \
    Toolkit/RSPiX/ORANGE/ImageTools/lasso.h \
    Toolkit/RSPiX/ORANGE/Laymage/laymage.h \
    Toolkit/RSPiX/ORANGE/Meter/meter.h \
    Toolkit/RSPiX/ORANGE/MsgBox/MsgBox.h \
    Toolkit/RSPiX/ORANGE/MultiGrid/MultiGrid.h \
    Toolkit/RSPiX/ORANGE/MultiGrid/MultiGridIndirect.h \
    Toolkit/RSPiX/ORANGE/Parse/SimpleBatch.h \
    Toolkit/RSPiX/ORANGE/Props/Props.h \
    Toolkit/RSPiX/ORANGE/QuickMath/FixedPoint.h \
    Toolkit/RSPiX/ORANGE/QuickMath/Fractions.h \
    Toolkit/RSPiX/ORANGE/QuickMath/QuickMath.h \
    Toolkit/RSPiX/ORANGE/QuickMath/VectorMath.h \
    Toolkit/RSPiX/ORANGE/RString/rstring.h \
    Toolkit/RSPiX/ORANGE/str/str.h \
    Toolkit/RSPiX/RSPiX.h \
    Toolkit/RSPiX/RSPiXUnix.h \
    Toolkit/WishPiX/Menu/menu.h \
    Toolkit/WishPiX/Prefs/prefline.h \
    Toolkit/WishPiX/Prefs/prefs.h \
    Toolkit/WishPiX/ResourceManager/resmgr.h \
    Toolkit/WishPiX/Spry/spry.h \
    Toolkit/variant.h \
    Toolkit/RSPiX/ORANGE/GameLib/AnimatedSprite.h \
    Toolkit/RSPiX/ORANGE/GameLib/Sprite.h \
    Toolkit/RSPiX/ORANGE/CDT/Queue.h \
    Toolkit/RSPiX/GREEN/BLiT/Blit.h \
    Toolkit/RSPiX/GREEN/BLiT/RFont.h \
    Toolkit/RSPiX/GREEN/BLiT/BlitInternal.h \
    Toolkit/RSPiX/GREEN/Image/Pal.h \
    Toolkit/RSPiX/GREEN/Hot/Hot.h \
    Toolkit/RSPiX/GREEN/BLiT/AlphaBlit.h

SOURCES += \
    Network/ProtoBSDIP.cpp \
    Realm.cpp \
    RSPiX.cpp \
    SampleMaster.cpp \
    Scene.cpp \
    Scoreboard.cpp \
    Settings.cpp \
    Smash.cpp \
    Network/Socket.cpp \
    StockPile.cpp \
    TexEdit.cpp \
    Thing.cpp \
    Title.cpp \
    Toolbar.cpp \
    TriggerRegions.cpp \
    Update.cpp \
    AIVars.cpp \
    Alpha.cpp \
    AlphaBlit.cpp \
    Animated3D.cpp \
    BufQ.cpp \
    Bulletfest.cpp \
    Camera.cpp \
    Crawler.cpp \
    Credits.cpp \
    Cutscene.cpp \
    Encrypt.cpp \
    Game.cpp \
    GameSettings.cpp \
    GameTime.cpp \
    Grip.cpp \
    IdBank.cpp \
    Input.cpp \
    InputSettings.cpp \
    InputSettingsDlg.cpp \
    Keys.cpp \
    Localize.cpp \
    Log.cpp \
    LogTab.cpp \
    main.cpp \
    MemFileFest.cpp \
    Menus.cpp \
    MenuSettings.cpp \
    MenuTrans.cpp \
    Network/Net.cpp \
    Organ.cpp \
    Personatorium.cpp \
    Play.cpp \
    Thing/AnimatedThing.cpp \
    Thing/Ball.cpp \
    Thing/Bouy.cpp \
    Thing/Chunk.cpp \
    Thing/Demon.cpp \
    Thing/Dispenser.cpp \
    Thing/Explode.cpp \
    Thing/Fire.cpp \
    Thing/GameEdit.cpp \
    Thing/GoalTimer.cpp \
    Thing/Hood.cpp \
    Thing/NavigationNet.cpp \
    Thing/Pylon.cpp \
    Thing/SoundRelay.cpp \
    Thing/SoundThing.cpp \
    Thing/Thing3D.cpp \
    Thing/Trigger.cpp \
    Thing/Warp.cpp \
    Thing/Weapon.cpp \
    Thing/Weapon/DeathWad.cpp \
    Thing/Weapon/Fireball.cpp \
    Thing/Weapon/Firebomb.cpp \
    Thing/Weapon/Grenade.cpp \
    Thing/Weapon/Heatseeker.cpp \
    Thing/Weapon/Mine.cpp \
    Thing/Weapon/Napalm.cpp \
    Thing/Weapon/Rocket.cpp \
    Thing/Thing3D/Barrel.cpp \
    Thing/Thing3D/Character.cpp \
    Thing/Thing3D/Flag.cpp \
    Thing/Thing3D/FlagBase.cpp \
    Thing/Thing3D/Item3D.cpp \
    Thing/Thing3D/Item3D/PowerUp.cpp \
    Thing/Thing3D/Character/Doofus.cpp \
    Thing/Thing3D/Character/Dude.cpp \
    Thing/Thing3D/Character/Doofus/Band.cpp \
    Thing/Thing3D/Character/Doofus/Ostrich.cpp \
    Thing/Thing3D/Character/Doofus/Person.cpp \
    Thing/Thing3D/Character/Doofus/Sentry.cpp \
    Toolkit/RSPiX/BLUE/unix/Bdebug.cpp \
    Toolkit/RSPiX/BLUE/unix/Bdisp.cpp \
    Toolkit/RSPiX/BLUE/unix/Bjoy.cpp \
    Toolkit/RSPiX/BLUE/unix/Bkey.cpp \
    Toolkit/RSPiX/BLUE/unix/Bmain.cpp \
    Toolkit/RSPiX/BLUE/unix/Bmouse.cpp \
    Toolkit/RSPiX/BLUE/unix/Bsound.cpp \
    Toolkit/RSPiX/BLUE/unix/Btime.cpp \
    Toolkit/RSPiX/CYAN/Unix/uColors.cpp \
    Toolkit/RSPiX/CYAN/Unix/uDialog.cpp \
    Toolkit/RSPiX/CYAN/Unix/uPath.cpp \
    Toolkit/RSPiX/GREEN/3D/pipeline.cpp \
    Toolkit/RSPiX/GREEN/3D/render.cpp \
    Toolkit/RSPiX/GREEN/3D/types3d.cpp \
    Toolkit/RSPiX/GREEN/3D/zbuffer.cpp \
    Toolkit/RSPiX/GREEN/BLiT/Fspr1.cpp \
    Toolkit/RSPiX/GREEN/BLiT/FSPR8.cpp \
    Toolkit/RSPiX/GREEN/BLiT/Rotate96.cpp \
    Toolkit/RSPiX/GREEN/BLiT/RPrint.cpp \
    Toolkit/RSPiX/GREEN/BLiT/ScaleFlat.cpp \
    Toolkit/RSPiX/GREEN/Image/Image.cpp \
    Toolkit/RSPiX/GREEN/Image/ImageFile.cpp \
    Toolkit/RSPiX/GREEN/Image/PalFile.cpp \
    Toolkit/RSPiX/GREEN/InputEvent/InputEvent.cpp \
    Toolkit/RSPiX/GREEN/Mix/mix.cpp \
    Toolkit/RSPiX/GREEN/Mix/MixBuf.cpp \
    Toolkit/RSPiX/GREEN/Sample/sample.cpp \
    Toolkit/RSPiX/GREEN/Snd/snd.cpp \
    Toolkit/RSPiX/GREEN/SndFx/SndFx.cpp \
    Toolkit/RSPiX/GREEN/Task/task.cpp \
    Toolkit/RSPiX/ORANGE/Attribute/attribute.cpp \
    Toolkit/RSPiX/ORANGE/Channel/channel.cpp \
    Toolkit/RSPiX/ORANGE/Chips/chip.cpp \
    Toolkit/RSPiX/ORANGE/color/colormatch.cpp \
    Toolkit/RSPiX/ORANGE/color/dithermatch.cpp \
    Toolkit/RSPiX/ORANGE/Debug/profile.cpp \
    Toolkit/RSPiX/ORANGE/DirtRect/DirtRect.cpp \
    Toolkit/RSPiX/ORANGE/File/file.cpp \
    Toolkit/RSPiX/ORANGE/GameLib/Region.cpp \
    Toolkit/RSPiX/ORANGE/GUI/btn.cpp \
    Toolkit/RSPiX/ORANGE/GUI/dlg.cpp \
    Toolkit/RSPiX/ORANGE/GUI/edit.cpp \
    Toolkit/RSPiX/ORANGE/GUI/guiItem.cpp \
    Toolkit/RSPiX/ORANGE/GUI/ListBox.cpp \
    Toolkit/RSPiX/ORANGE/GUI/ListContents.cpp \
    Toolkit/RSPiX/ORANGE/GUI/MultiBtn.cpp \
    Toolkit/RSPiX/ORANGE/GUI/ProcessGui.cpp \
    Toolkit/RSPiX/ORANGE/GUI/PushBtn.cpp \
    Toolkit/RSPiX/ORANGE/GUI/scrollbar.cpp \
    Toolkit/RSPiX/ORANGE/GUI/txt.cpp \
    Toolkit/RSPiX/ORANGE/IFF/iff.cpp \
    Toolkit/RSPiX/ORANGE/ImageTools/lasso.cpp \
    Toolkit/RSPiX/ORANGE/Laymage/laymage.cpp \
    Toolkit/RSPiX/ORANGE/Meter/meter.cpp \
    Toolkit/RSPiX/ORANGE/MsgBox/MsgBox.cpp \
    Toolkit/RSPiX/ORANGE/MultiGrid/MultiGrid.cpp \
    Toolkit/RSPiX/ORANGE/MultiGrid/MultiGridIndirect.cpp \
    Toolkit/RSPiX/ORANGE/Parse/SimpleBatch.cpp \
    Toolkit/RSPiX/ORANGE/QuickMath/FixedPoint.cpp \
    Toolkit/RSPiX/ORANGE/QuickMath/QuickMath.cpp \
    Toolkit/RSPiX/ORANGE/RString/rstring.cpp \
    Toolkit/RSPiX/ORANGE/str/str.cpp \
    Toolkit/WishPiX/Menu/menu.cpp \
    Toolkit/WishPiX/Prefs/prefline.cpp \
    Toolkit/WishPiX/Prefs/prefs.cpp \
    Toolkit/WishPiX/ResourceManager/resmgr.cpp \
    Toolkit/WishPiX/Spry/spry.cpp \
    Toolkit/RSPiX/ORANGE/GameLib/AnimatedSprite.cpp \
    Toolkit/RSPiX/ORANGE/GameLib/Shapes.cpp \
    Toolkit/RSPiX/ORANGE/GameLib/Sprite.cpp \
    Toolkit/RSPiX/ORANGE/CDT/Queue.cpp \
    Toolkit/RSPiX/ORANGE/CDT/List.cpp \
    Toolkit/RSPiX/GREEN/BLiT/RFont.cpp \
    Toolkit/RSPiX/GREEN/Image/ImageConvert.cpp \
    Toolkit/RSPiX/GREEN/Image/Pal.cpp \
    Toolkit/RSPiX/GREEN/Hot/Hot.cpp \
    Toolkit/RSPiX/GREEN/BLiT/AlphaBlit.cpp \
    Toolkit/RSPiX/GREEN/BLiT/Blit.cpp \
    Toolkit/RSPiX/GREEN/BLiT/BlitInternal.cpp \
    Toolkit/RSPiX/GREEN/BLiT/Line.cpp \
    Toolkit/RSPiX/GREEN/BLiT/Mono.cpp \
    Toolkit/RSPiX/GREEN/BLiT/BlitTransparent.cpp

win32:HEADERS += \
    Network/NetBrowse.h \
    Network/NetClient.h \
    Network/NetDlg.h \
    Network/NetInput.h \
    Network/NetMsgr.h \
    Network/NetServer.h

win32:SOURCES += \
    Network/NetBrowse.cpp \
    Network/NetClient.cpp \
    Network/NetDlg.cpp \
    Network/NetMsgr.cpp \
    Network/NetServer.cpp
