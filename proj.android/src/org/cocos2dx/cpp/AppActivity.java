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
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.Toast;

public class AppActivity extends Cocos2dxActivity {
    private long mkeyTime = 0;
    static {
        MobClickCppHelper.loadLibrary();
    }

    BaiduBanner mBaiduBanner_Image;
    BaiduBanner mBaiduBanner_Image_Text;

    // 调用广告
    private Handler aHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
            case -1:
                addBannerAd();//百通横幅
                break;
            case -2:
                addInstertAd();//百通插屏
                break;
            case -3:
                deleteBannerAd();
                break;
            default:
                showShare(msg.what);
                break;
            }
        }
    };
    
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

        // 百通广告
        AppUnionSDK.getInstance(AppActivity.this).initSdk();
      //插屏预加载代码
        AppUnionSDK.getInstance(AppActivity.this)
        .loadInterstitialAd(AppActivity.this,
                new InterstitialAdListener() {

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

    private void showShare(int n) {
        ShareSDK.initSDK(this);
        OnekeyShare oks = new OnekeyShare();
        // 关闭sso授权
        oks.disableSSOWhenAuthorize();
        // 分享时Notification的图标和文字 2.5.9以后的版本不调用此方法
        // oks.setNotification(R.drawable.ic_launcher,getString(R.string.app_name);
        // title标题，印象笔记、邮箱、信息、微信、人人网和QQ空间使用
        oks.setTitle("小球特烦恼");
        // titleUrl是标题的网络链接，仅在人人网和QQ空间使用
        oks.setTitleUrl("http://a.app.qq.com/o/simple.jsp?pkgname=com.liubo.DayDayUp");
        // text是分享文本，所有平台都需要这个字段
        oks.setText("玩了下《小球特烦恼》这个游，感觉超虐啊，得" + n + "分真不容易，来炫耀下！");
        // imagePath是图片的本地路径，Linked-In以外的平台都支持此参数
        // String url="file:///android_asset/DayDayUp.png";
        // oks.setImagePath(url);// 确保SDcard下面存在此张图片
        oks.setImageUrl("https://raw.githubusercontent.com/nangongyifeng/SharePhoto/master/DayDayUp.png");
        // url仅在微信（包括好友和朋友圈）中使用
        oks.setUrl("http://a.app.qq.com/o/simple.jsp?pkgname=com.liubo.DayDayUp");
        // comment是我对这条分享的评论，仅在人人网和QQ空间使用
        oks.setComment("还不错哦！");
        // site是分享此内容的网站名称，仅在QQ空间使用
        oks.setSite("小球特烦恼");
        // siteUrl是分享此内容的网站地址，仅在QQ空间使用
        oks.setSiteUrl("http://a.app.qq.com/o/simple.jsp?pkgname=com.liubo.DayDayUp");

        // 启动分享GUI
        oks.show(this);
    }

    /**
     * 分享功能
     * 
     * @param context
     *            上下文
     * @param activityTitle
     *            Activity的名字
     * @param msgTitle
     *            消息标题
     * @param msgText
     *            消息内容
     * @param f
     *            图片路径，不分享图片则传null
     */
    public static void shareMsg(Context context, String activityTitle, String msgTitle, String msgText,
            String filePath) {
        Intent intent = new Intent(Intent.ACTION_SEND);
        if (filePath == null || filePath.equals("")) {
            intent.setType("text/plain"); // 纯文本
        } else {
            File f = new File(filePath);
            System.out.println(f);
            if (f != null) {
                intent.setType("image/*");
                Uri u = Uri.fromFile(f);
                intent.putExtra(Intent.EXTRA_STREAM, u);
            }
        }
        intent.putExtra(Intent.EXTRA_SUBJECT, msgTitle);
        intent.putExtra(Intent.EXTRA_TEXT, msgText);
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        context.startActivity(Intent.createChooser(intent, activityTitle));
    }
    
    // 分享照片
    public void SharePhoto(String photoUri, final Activity activity) {
        Intent shareIntent = new Intent(Intent.ACTION_SEND);
        File file = new File(photoUri);
        shareIntent.putExtra(Intent.EXTRA_STREAM, Uri.fromFile(file));
        shareIntent.setType("image/jpeg");
        startActivity(Intent.createChooser(shareIntent, activity.getTitle()));
    }

    // 双击退出
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK) {
            if ((System.currentTimeMillis() - mkeyTime) > 2000) {
                mkeyTime = System.currentTimeMillis();
                Toast.makeText(this, "再按一次退出", Toast.LENGTH_LONG).show();
                onDestroy();
            } else {
                finish();
                System.exit(0);
            }
            return false;
        }
        return super.onKeyDown(keyCode, event);
    }

    //百通
    public void addBannerAd() {
        if (mBaiduBanner_Image == null
                || mBaiduBanner_Image.getVisibility() == View.GONE) {

            if (mBaiduBanner_Image_Text != null) {
                mBaiduBanner_Image_Text
                        .setVisibility(View.GONE);
            }

            FrameLayout.LayoutParams lytp = new FrameLayout.LayoutParams(
                    ViewGroup.LayoutParams.WRAP_CONTENT,
                    ViewGroup.LayoutParams.WRAP_CONTENT);
            lytp.gravity = Gravity.BOTTOM
                    | Gravity.CENTER_HORIZONTAL;
            mBaiduBanner_Image = new BaiduBanner(
                    AppActivity.this);
            mBaiduBanner_Image
                    .setBannerType(BannerType.IMAGE_ONLY);
            AppActivity.this.addContentView(
                    mBaiduBanner_Image, lytp);
        }
    }

    public void deleteBannerAd(){
        if(mBaiduBanner_Image != null){
            mBaiduBanner_Image.setVisibility(View.GONE);
        }
    }

    public void addInstertAd() {
        System.out.println("插屏广告");
        if (AppUnionSDK.getInstance(AppActivity.this)
                .isInterstitialAdReady()) {
            AppUnionSDK.getInstance(AppActivity.this)
                    .showInterstitialAd();
            System.out.println("展示插屏广告");
        }
    }

    @Override
    protected void onDestroy() {
        // TODO Auto-generated method stub
        AppUnionSDK.getInstance(this).quitSdk();
        super.onDestroy();
    }
}
