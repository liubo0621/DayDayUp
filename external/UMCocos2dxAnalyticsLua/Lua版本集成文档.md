#cocos2d-x统计分析SDK使用指南

1. 在项目中集成SDK
1.1. Andorid
导入SDK
先在友盟的管理后台中创建App，获得AppKey
下载SDK并解压缩
导入插件

下载最新版SDK的zip包，将其中的mobclickcpphelper.jar文件复制到本地工程libs子目录下,将libMobClickCpp文件夹复制到[COCOS2DX_ROOT]/cocos2dx/platform/third_party/android/prebuilt目录下（cocos2d-x 3.0以上这个目录是[COCOS2DX_ROOT]/external） 将其中的（lua_binding.h,lua_binding.cpp,MobClickCpp.h）复制到工程目录下（例如：Classes目录下，2.0以上版本还需要拷贝LuaBasicConversions.cpp,LuaBasicConversions.h文件）

配置

Eclipse用户右键工程根目录，选择Properties -> Java Build Path -> Libraries，然后点击Add External JARs...，选择指向jar的路径，点击OK

注意

Eclipse ADT 17 以上版本用户，不需要这个步骤，完成第一步的合并libs文件夹之后，在Eclipse里面刷新一下工程就好了。不要通过上述步骤手动添加jar包引用。 详情请参考Dealing with dependencies in Android projects.
导入库文件

修改你的工程Android.mk文件在LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static后添加LOCAL_SHARED_LIBRARIES := mobclickcpp_shared，
在$(call import-module,cocos2dx)后添加$(call import-module,libmobclickcpp)。
在LOCAL_SRC_FILES后添加lua_binding.cpp(2.0还需要添加LuaBasicConversions.cpp)。
导入完成

配置 AndroidManifest.xml
<uses-permission android:name="android.permission.ACCESS_NETWORK_STATE"></uses-permission>
<uses-permission android:name="android.permission.ACCESS_WIFI_STATE" />
<uses-permission android:name="android.permission.INTERNET"></uses-permission>
<uses-permission android:name="android.permission.READ_PHONE_STATE"></uses-permission>
####添加JAVA代码

在游戏主Activity的onCreate方法中中加入如下代码：
MobClickCppHelper.init(this);
添加static代码块来load c++的library：
static {
        MobClickCppHelper.loadLibrary();
}
修改后的代码示例：

public class AppActivity extends Cocos2dxActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState){
            super.onCreate(savedInstanceState);
            MobClickCppHelper.init(this);
    }

    static {
        MobClickCppHelper.loadLibrary();
    }
}
如果使用的是cocos2d-x 2.x.x 的版本，请把MobClickCppHelper.loadLibrary();添加到System.loadLibrary("xxx");前面，例如：

static {
        MobClickCppHelper.loadLibrary();
        System.loadLibrary("hellocpp");//“hellocpp”替换成自己的shared library名称
}
修改后的代码示例：

public class AppActivity extends Cocos2dxActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState){
            super.onCreate(savedInstanceState);
            MobClickCppHelper.init(this);
    }

    static {
        MobClickCppHelper.loadLibrary();
        System.loadLibrary("hellocpp");//“hellocpp”替换成自己的shared library名称
    }
}
1.2. iOS
导入SDK
解压缩压缩包。
导入插件

所需文件： libMobClickCpp.a MobClickCpp.h lua_binding.h lua_binding.cpp (2.0以上版本需要 LuaBasicConversions.h LuaBasicConversions.cpp)

本SDK支持的iOS版本为5.0以上，请修改General-->Deployment Target的版本为5.0以上。

修改c++标准库选项：游戏工程文件-->游戏对应TARGET-->Build Settings-->C++ Standard Library-->libc++。 cocos2d-x静态库的工程也要修改：cocos2dx.xcodeproj-->cocos2dx-->Build Settings-->C++ Standard Library-->libc++。 添加lua库：游戏工程文件-->游戏对应TARGET-->Build Settings-->Search Paths-->Library Search Paths,将lua库的路径添加进去，lua库在引擎目录下scripting/lua/lua/ios/。

注意

cocos2d-x 3.0以上版本的开发者使用默认设置即可，不需要以上三个步骤
请在你的工程目录结构中，右键选择Add->Existing Files…，选择这几个文件。或者将这几个文件拖入XCode工程目录结构中，在弹出的界面中勾选Copy items into destination group's folder(if needed), 并确保Add To Targets勾选相应的target。

如果你的工程没有使用libcurl库，还要把[COCOS2DX_ROOT]/cocos2dx/platform/third_party/ios/libraries/libcurl.a（cocos2d-x 3.0以上请替换为[COCOS2DX_ROOT]/external/curl/prebuilt/ios/libcurl.a）按照上面的方法加入到工程中。
2. 使用SDK
在AppDelegate类中加入如下代码：
#include "MobClickCpp.h"
#include "lua_binding.h"
在applicationDidFinishLaunching方法中加入
lua_register_mobclick_module(lua_State* L);
在applicationDidEnterBackground方法中加入
umeng::MobClickCpp::applicationDidEnterBackground();
在applicationWillEnterForeground方法中加入
umeng::MobClickCpp::applicationWillEnterForeground();
lua接口使用注意

请在CCDirector:sharedDirector():endToLua();结束游戏前调用
MobClickForLua.umeng_end_lua()
3. 高级功能集成指南
所需文件 MobClickForLua.lua,将其拷贝到lua文件的查找目录中（src，res等），（ios还需要将其添加到xcode工程中）

MobClickForLua.lua 是MobClickCpp的lua接口

使用实例如下：require("MobClickForLua")

启动sdk统计
在你的lua文件的入口出使用接口MobClickForLua.startMobclick("你的appkey","你的渠道id") 例如3.0以上版本demo的lua入口文件是main.lua,2.0以上版本demo的入口文件是hello.lua

3.1. 使用自定义事件
使用自定义事件功能请先在网站应用管理后台(设置->编辑自定义事件)中添加相应的自定义事件后，服务器才会对相应的自定义事件请求进行处理。这里我们将提供几个简单而通用的接口：

3.1.1. 事件数量统计
在您希望跟踪的代码部分，调用如下方法：

umeng::MobClickCpp::event(const char * eventId, const char * label = NULL)

对应的lua接口是MobClickForLua.event(eventId,...)
eventId为当前统计的事件ID。

label为当前事件的属性和取值（键值对）。

示例：统计微博应用中"转发"事件发生的次数，那么在转发的函数里调用

c++:umeng::MobClickCpp::event("Forward");
lua:MobClickForLua.event("Forward")
示例： 统计游戏中“死亡”事件发生的关卡数，那么可以在死亡的函数里调用

c++:umeng::MobClickCpp::event("player_dead", "level");
lua:MobClickForLua.event("player_dead", "level")
考虑事件在一个属性上的取值，可以调用如下方法：

umeng::MobClickCpp::event(const char *eventId, eventDict *attributes)
attributes为当前事件的属性和取值（键值对）。

示例：统计电商应用中“购买”事件发生的次数，以及购买的商品类型及数量，那么在购买的函数里调用：

c++:umeng::eventDict dict;
    dict["type"] = "book";
    dict["quantity"] = "3";
    umeng::MobClickCpp::event("purchase", &dict);
lua:local dict = {}
    dict["type"] = "book";
    dict["quantity"] = "3";
    MobClickForLua.event("purchase", dict)
3.1.2. 计算事件
使用计算事件需要在后台添加事件时选择“计算事件”。 #####1. 统计数值型变量的值的分布 统计一个数值类型的连续变量，用户每次触发的数值的分布情况，如事件持续时间、每次付款金额等，可以调用如下方法：

c++:umeng::MobClickCpp::event(const char *eventId, eventDict *attributes, int counter);
lua:MobClickForLua.event(eventId,dict,counter)
2. 统计点击次数及各属性触发次数
计算事件除能够统计数值型的参数外还具有计数事件的所有功能。详细说明。

3.1.3. 注意事项
1. event_id 和 tag 不能使用特殊字符，且长度不能超过128个字节；map中的key和value 都不能使用特殊字符，key 不能超过128个字节，value 不能超过256个字节
2. key不能含有“,”字符，value不能含有"|"字符
这是因为在不同shared library之间传递c++的std对象可能引发兼容性问题，所以需要先将std对象转换成c语言的基本类型

在这里我会把eventDict转换成“k1,v1|k2,v2”形式的字符串

3. id， ts， du是保留字段，不能作为eventId及key的名称。
4. 每个应用至多添加500个自定义事件，每个event 的 key不能超过10个，每个key的取值不能超过1000个（不允许通过key-value结构来统计类似搜索关键词，网页链接等随机生成的字符串信息）。 如有任何问题，请联系客服qq: 800083942。
3.2. 使用分发渠道分析
有时需要统计应用程序的分发渠道，例如有多少用户来从应用汇 下载了您的应用,又有多少用户通过GooglePlay下载到您的应用程序。您需要在调用MOBCLICKCPP_START_WITH_APPKEY_AND_CHANNEL时使用channelId参数。

注意：
每台设备仅记录首次安装激活的渠道，在其他渠道再次安装不会重复计量。 所以在测试不同的渠道的时候，请使用不同的设备来分别测试。渠道id注意不能是纯数字(eg.value="GooglePlay")。
3.3. 使用在线配置功能
这个功能目前可以帮您在网站上动态配置的自定义key-value型的键值对

您需要在MOBCLICKCPP_START_WITH_APPKEY_AND_CHANNEL后调用

umeng::MobClickCpp::updateOnlineConfig();
您也可以传递更新的回调作为参数

c++:umeng::MobClickCpp::updateOnlineConfig(MobClickOnlineConfigUpdateDelegate* pDelegate, void *pUserData);

lua:MobClickForLua.updateOnlineConfig(...)
您需要一个继承自MobClickOnlineConfigUpdateDelegate的类的对象,请自行保证pDelegate在更新在线参数过程中不会被delete，例如：

class HelloWorld : public cocos2d::CCLayer, public umeng::MobClickOnlineConfigUpdateDelegate {
    //HelloWorld的其他成员
public:
    virtual void onUpdateFinished(bool succeed, void* pUserData){
        //您的代码...

        //因为调用updateOnlineConfig之前retain了 所以release
        release();
    }
};


void HelloWorld::someMethod(){
    //需要保证this在更新在线参数过程中不会被delete
    this->retain();

    umeng::MobClickCpp::updateOnlineConfig(this);
}


lua:具体请参考lua_binding.cpp中的注释部分(在线参数更新测试部分在文件最下边)
这句代码从服务器获取在线参数，并缓存本地。 当在项目里需要获取某个具体参数时调用

c++:string value = umeng::MobClickCpp::getConfigParam(const char *key);
lua:local value = MobClickForLua.getConfigParam(key);
key为友盟服务器上事先设置好的key。

3.4. 使用页面访问统计
您可以统计每个View停留时长及路径。

需要配对使用,如

    c++:umeng::MobClickCpp::beginLogPageView("PageOne");
    lua:MobClickForLua.beginLogPageView("PageOne");
    c++:umeng::MobClickCpp::endLogPageView("PageOne");
    lua:MobClickForLua.endLogPageView("PageOne");
注意： 
* 我们针对页面访问进行的是抽样统计，所以只有数据量达到一定规模之后才可以看到。

* 由于这组方法的调用，需要保证线性不交叉，每个 start 都有一个 end 配对，如下：
    onPageStart ->onPageEnd-> onPageStart -> onPageEnd -> onPageStart ->onPageEnd
    这样才能保证每个页面统计的正确。
3.5. 游戏方法详解
1.玩家账户设置
当玩家账户确定或更改时，尽早调用以下两个方法来设置
// 设置玩家等级.
c++:void umeng::MobClickCpp::setUserLevel(const char *level);
lua:MobClickForLua.setUserLevel( level )
// 设置玩家属性.
C++:void umeng::MobClickCpp::setUserInfo(const char * userId, Sex sex, int age, const char * platform);
lua:MobClickForLua.setUserInfo(userId,sex,age,platform)
字段	含义	取值
level	玩家等级	非空字符串
userId	玩家Id	玩家在游戏中的唯一ID，如果可能请尽早确定
sex	性别	unknown male female
age	年龄	0..200 整数
platform	来源	
2.充值
玩家支付货币兑换虚拟币,用于统计游戏的收入情况
// 充值.
C++:void umeng::MobClickCpp::pay(double cash, int source, double coin);
lua:MobClickForLua.pay(cash,source,...)
// 充值并购买道具.
C++:void umeng::MobClickCpp::pay(double cash, int source, const char * item, int amount, double price);
lua:MobClickForLua.pay(cash,source,...)
字段	含义	取值
cash	真实币数量	>=1的整数,最多只保存小数点后2位
source	支付渠道	1 ~ 99的整数, 其中1..8 是预定义含义,其余数值需要在网站设置。
coin	虚拟币数量	大于0的整数, 最多只保存小数点后2位,一般情况下coin = amount * price
item	道具名称	非空字符串
amount	道具数量	大于0的整数
price	道具单价	>=0
source对应的数字、含义如下表：

数值	含义
1	App Store
2	支付宝
3	网银
4	财付通
5	移动通信
6	联通通信
7	电信通信
8	paypal
Demo 在游戏中充值或者购买虚拟币的时候调用此方法，比如通过支付宝用 10元钱 购买了 1000 个金币，可以这样调用：
C++:umeng::MobClickCpp::pay(10, 2, 1000); lua:MobClickForLua.pay(10.2.1000);

有些时候在游戏中会直接购买某个道具，比如10元购买 2个魔法药水,每个药水50个金币，可以调用下面的方法在付费的同时购买道具。

C++:umeng::MobClickCpp::pay(10, 2, "magic_bottle", 2, 500); lua:MobClickForLua.pay(10, 2, "magic_bottle", 2, 500)

3.购买
玩家用虚拟币兑换一定数量、价值的道具
// 购买道具.
C++:void umeng::MobClickCpp::buy(const char *item, int amount, double price);
lua:MobClickForLua.buy( item,amount,price );
字段	含义	取值
item	道具名称	非空字符串
amount	道具数量	大于0的整数
price	道具单价	>=0
购买道具需要传递道具ID(名称)、数量(amount)、虚拟币数量coin(amount * price)。

Demo
在游戏中使用金币购买了1个头盔，一个头盔价值 1000 金币，可以这样统计：

C++:umeng::MobClickCpp::buy("helmet", 1, 1000); lua:MobClickForLua.buy("helmet", 1, 1000)

4.消耗
玩家使用道具的情况
// 购买道具.
C++:void umeng::MobClickCpp::use(const char *item, int amount, double price);
lua:MobClickForLua.use( item,amount,price )
字段	含义	取值
item	道具名称	非空字符串
amount	道具数量	大于0的整数
price	道具单价	>=0
消耗道具需要传递道具ID(名称)、数量(amount)、虚拟币数量coin(amount * price)。

Demo
游戏中的物品损耗，比如使用了2瓶魔法药水,每个需要50个虚拟币，可以这样统计：

C++:umeng::MobClickCpp::use("magic_bottle", 2, 50); lua:MobClickForLua.use("magic_bottle", 2, 50)

5.奖励
游戏中发生的金币、道具赠送行为
// 赠送金币.
C++:void umeng::MobClickCpp::bonus(double coin, int source);
lua:MobClickForLua.bonus( ... )
// 赠送道具.
C++:void umeng::MobClickCpp::bonus(const char *item, int amount, double price, int source);
lua:MobClickForLua.bonus( ... )
字段	含义	取值
coin	虚拟币数量	大于0的整数, 最多只保存小数点后2位
source	奖励渠道	取值在 1~99 之间， 1~3 已经被预先定义， 其余数值需要在网站设置含义
item	道具名称	非空字符串
amount	道具数量	大于0的整数
price	道具单价	>=0
消耗道具需要传递道具ID(名称)、数量(amount)、虚拟币数量coin(amount * price)。
source对应的数字、含义如下表：

数值	含义
1	玩家赠送
2	开发商赠送
3	游戏奖励
6.关卡
记录玩家在游戏中的进度
// 进入关卡.
C++:void umeng::MobClickCpp::startLevel(const char *level);
Lua:MobClickForLua.startLevel(level)
// 通过关卡.
C++:void umeng::MobClickCpp::finishLevel(const char *level);
Lua:MobClickForLua.finishLevel(level)
// 未通过关卡.
C++:void umeng::MobClickCpp::failLevel(const char *level);
lua:MobClickForLua.failLevel( level )
字段	含义	取值
level	关卡id	非空字符串(eg：level-01)
开发者需要在关卡状态变动时调用startLevel方法重新设置当前关卡。 多次调用startLevel，以最后设置的为准
在游戏开启新的关卡的时候调用 startLevel: 方法，在关卡失败的时候调用 failLevel: 方法，在成功过关的时候调用 finishLevel: 方法。

调用 failLevel: 或 finishLevel: 的时候会计算从 startLevel 开始的时长，作为这一关卡的耗时。level 字段最好为非空可排序的字符串。

SDK 默认减去程序切入后台的时间。

level参数不允许传入NULL。

3.6. 其他
C++:umeng::MobClickCpp::setLogEnabled(bool value) 设置为true,umeng SDK 会输出log信息,记得release产品时要设置回false。
lua:MobClickForLua.setLogEnabled(value)

C++:umeng::MobClickCpp::setEncryptEnabled(bool value) 设置为true,umeng SDK umeng SDK 会将日志信息做加密处理，要在MOBCLICKCPP_START_WITH_APPKEY之前调用`

lua:MobClickForLua.setEncryptEnabled(value)

如果安卓项目使用了proguard做代码混淆，请在proguard配置文件中加入-keep class com.umeng.mobclickcpp.**{ *; }。