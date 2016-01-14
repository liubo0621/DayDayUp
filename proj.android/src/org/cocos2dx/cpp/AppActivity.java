/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.cpp;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import org.cocos2dx.lib.Cocos2dxActivity;

import com.baidu.ops.appunion.sdk.AppUnionSDK;
import com.baidu.ops.appunion.sdk.InterstitialAdListener;
import com.baidu.ops.appunion.sdk.banner.BaiduBanner;
import com.baidu.ops.appunion.sdk.banner.BannerType;
import com.qq.e.ads.banner.ADSize;
import com.qq.e.ads.banner.AbstractBannerADListener;
import com.qq.e.ads.banner.BannerView;
import com.qq.e.ads.interstitial.AbstractInterstitialADListener;
import com.qq.e.ads.interstitial.InterstitialAD;
import com.qq.e.ads.splash.SplashAD;
import com.qq.e.ads.splash.SplashADListener;
import com.umeng.mobclickcpp.MobClickCppHelper;
import com.umeng.update.UmengUpdateAgent;

import cn.sharesdk.framework.ShareSDK;
import cn.sharesdk.onekeyshare.OnekeyShare;

import android.R;
import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.Toast;
import android.widget.FrameLayout.LayoutParams;
import android.widget.RelativeLayout;

public class AppActivity extends Cocos2dxActivity {
    private long mkeyTime = 0;
    private static boolean isShowBTbanner = false;

    static {
        MobClickCppHelper.loadLibrary();
    }

    // 百通
    private BaiduBanner mBaiduBanner_Image;
    // 广点通
    private FrameLayout bannerContainer;
    private BannerView bv;
    private InterstitialAD iad;

    // 调用广告
    private Handler aHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
            case -1:
                showBannerAD();
                break;
            case -2:
                showInterstitialAD();
                break;
            case -3:
                deleteBannerAd();
                break;
            case -11:
                tip(-11);
                break;
            case -12:
                tip(-12);
                break;
            default:
                showShare(msg.what);
                break;
            }
        }
    };

    void tip(int n){
        if(n == -11) {
            Toast.makeText(this, "您还没选择模式哦！", Toast.LENGTH_LONG).show();
        }else if(n == -12) {
            Toast.makeText(this, "您还没选择难度哦", Toast.LENGTH_LONG).show();
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        // TODO Auto-generated method stub
        super.onCreate(savedInstanceState);
        // umeng 数据统计
        MobClickCppHelper.init(this);
        // umeng 自动更新
        UmengUpdateAgent.setUpdateOnlyWifi(false);
        UmengUpdateAgent.update(this);
        JNIHelper.init(aHandler);

        // 初始化广告
        initBT();

        addBTInterstitialAD();
    }

    private void showShare(int n) {
        ShareSDK.initSDK(this);
        OnekeyShare oks = new OnekeyShare();
        // 关闭sso授权
        oks.disableSSOWhenAuthorize();
        // 分享时Notification的图标和文字 2.5.9以后的版本不调用此方法
        // oks.setNotification(R.drawable.ic_launcher,getString(R.string.app_name);
        // title标题，印象笔记、邮箱、信息、微信、人人网和QQ空间使用
        oks.setTitle("一发不止");
        // titleUrl是标题的网络链接，仅在人人网和QQ空间使用
        oks.setTitleUrl("http://a.app.qq.com/o/simple.jsp?pkgname=com.liubo.DayDayUp");
        // text是分享文本，所有平台都需要这个字段
        oks.setText("我在《一发不止》这个游戏中得了" + n + "分，还有谁？ 求超越！");
        // imagePath是图片的本地路径，Linked-In以外的平台都支持此参数
        // String url="file:///android_asset/DayDayUp.png";
        // oks.setImagePath(url);// 确保SDcard下面存在此张图片
        oks.setImageUrl("https://raw.githubusercontent.com/nangongyifeng/SharePhoto/master/cannot_stop.png");
        // url仅在微信（包括好友和朋友圈）中使用
        oks.setUrl("http://a.app.qq.com/o/simple.jsp?pkgname=com.liubo.DayDayUp");
        // comment是我对这条分享的评论，仅在人人网和QQ空间使用
        oks.setComment("还不错哦！");
        // site是分享此内容的网站名称，仅在QQ空间使用
        oks.setSite("一发不止");
        // siteUrl是分享此内容的网站地址，仅在QQ空间使用
        oks.setSiteUrl("http://a.app.qq.com/o/simple.jsp?pkgname=com.liubo.DayDayUp");

        // 启动分享GUI
        oks.show(this);
    }

    // 分享照片
    public void SharePhoto(String photoUri, final Activity activity) {
        Intent shareIntent = new Intent(Intent.ACTION_SEND);
        File file = new File(photoUri);
        shareIntent.putExtra(Intent.EXTRA_STREAM, Uri.fromFile(file));
        shareIntent.setType("image/jpeg");
        startActivity(Intent.createChooser(shareIntent, activity.getTitle()));
    }

    // 展示广告
    public void showBannerAD() {
//        addBTBannerAD();
//        isShowBTbanner = true;
//
//        if (mBaiduBanner_Image.getVisibility() == View.GONE) {
            addGDTbannerAD();
//            isShowBTbanner = false;
//        }
    }

    public void showInterstitialAD() {
        // TODO Auto-generated method stub
//        if (!isShowBTbanner) {
//            addGDTInterstitialAD();
//        }else{
            addBTInterstitialAD();
//        }
    }

    public void deleteBannerAd() {
        // TODO Auto-generated method stub
        doCloseGDTBanner();
        doCloseBTBannerAD();
    }

    // 百通
    private void initBT() {
        // 百通广告
        AppUnionSDK.getInstance(AppActivity.this).initSdk();
        // 插屏预加载代码
        AppUnionSDK.getInstance(AppActivity.this).loadInterstitialAd(AppActivity.this, new InterstitialAdListener() {

            @Override
            public void onAdReady() {
                // TODO Auto-generated method stub
            }

            @Override
            public void onAdPresent() {
                // TODO Auto-generated method stub

            }

            @Override
            public void onAdFailed(String reason) {
                // TODO Auto-generated method stub

            }

            @Override
            public void onAdDismissed() {
                // TODO Auto-generated method stub

            }
        }, false);

    }

    public void addBTBannerAD() {
        if (mBaiduBanner_Image == null || mBaiduBanner_Image.getVisibility() == View.GONE) {
            FrameLayout.LayoutParams lytp = new FrameLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT,
                    ViewGroup.LayoutParams.WRAP_CONTENT);
            lytp.gravity = Gravity.BOTTOM | Gravity.CENTER_HORIZONTAL;
            mBaiduBanner_Image = new BaiduBanner(AppActivity.this);
            mBaiduBanner_Image.setBannerType(BannerType.IMAGE_ONLY);
            AppActivity.this.addContentView(mBaiduBanner_Image, lytp);
        }
    }

    public void doCloseBTBannerAD() {
        if (mBaiduBanner_Image != null) {
            mBaiduBanner_Image.setVisibility(View.GONE);
        }
    }

    public void addBTInterstitialAD() {
        if (AppUnionSDK.getInstance(AppActivity.this).isInterstitialAdReady()) {
            AppUnionSDK.getInstance(AppActivity.this).showInterstitialAd();
        }
    }

    // 广点通
    private void initGDTBanner() {
        this.bv = new BannerView(this, ADSize.BANNER, Constants.APPID, Constants.BannerPosID);
        bv.setRefresh(30);
        bv.setADListener(new AbstractBannerADListener() {

            @Override
            public void onNoAD(int arg0) {
                Log.i("AD_DEMO", "BannerNoAD，eCode=" + arg0);
            }

            @Override
            public void onADReceiv() {
                Log.i("AD_DEMO", "ONBannerReceive");
            }
        });
        bannerContainer = new FrameLayout(this);
        FrameLayout.LayoutParams bannerContainerParame = new FrameLayout.LayoutParams(LayoutParams.WRAP_CONTENT,
                LayoutParams.WRAP_CONTENT);
        bannerContainerParame.gravity = Gravity.BOTTOM | Gravity.CENTER_HORIZONTAL;
        this.addContentView(bannerContainer, bannerContainerParame);
    }

    public void addGDTbannerAD() {
        if (bv == null) {
            initGDTBanner();
            bannerContainer.addView(bv);
        }
        bv.loadAD();
    }

    private void doCloseGDTBanner() {
        if (bv!=null) {
            bannerContainer.removeAllViews();
            bv.destroy();
            bv = null;
        }
    }

    public void addGDTInterstitialAD() {
        if (iad == null) {
            iad = new InterstitialAD(this, Constants.APPID, Constants.InterteristalPosID);
        }
        iad.setADListener(new AbstractInterstitialADListener() {

            @Override
            public void onNoAD(int arg0) {
                Log.i("AD_DEMO", "LoadInterstitialAd Fail:" + arg0);
            }

            @Override
            public void onADReceive() {
                iad.show();
            }
        });
        iad.loadAD();
    }

    // 退出
    // 双击退出
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        // if (keyCode == KeyEvent.KEYCODE_BACK) {
        // if ((System.currentTimeMillis() - mkeyTime) > 2000) {
        // mkeyTime = System.currentTimeMillis();
        // Toast.makeText(this, "再按一次退出", Toast.LENGTH_LONG).show();
        // onDestroy();
        // } else {
        // finish();
        // System.exit(0);
        // }
        // return false;
        // }
        // return super.onKeyDown(keyCode, event);
        return false;
    }

    @Override
    protected void onDestroy() {
        // TODO Auto-generated method stub
        super.onDestroy();
        // 百通
        AppUnionSDK.getInstance(this).quitSdk();

        // 销毁广点通banner
        bv.destroy();
        bv = null;
    }
}
