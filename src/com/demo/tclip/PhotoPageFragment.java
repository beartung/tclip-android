package com.demo.tclip;

import android.app.Activity;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;

import android.os.AsyncTask;
import android.os.Bundle;

import android.support.v4.app.Fragment;

import android.util.Log;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import android.widget.ImageView;

import com.opencv.TClip;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;

public class PhotoPageFragment extends Fragment{

    private static final String TAG = "PhotoPage";
    private static final int[] IDS = { R.id.img1, R.id.img2, R.id.img3, R.id.img4 };
    private static final int[] WIDTHS = { 300, 200, 150, 400 };
    private static final int[] HEIGHTS = { 240, 200, 300, 200 };
    private ImageView img;
    private ImageView[] imgs;
    private Bitmap[] bitmaps;
    private int photoId;
    private String config;

    public static PhotoPageFragment newInstance(String config, int photoId) {
        PhotoPageFragment f = new PhotoPageFragment();
        f.photoId = photoId;
        f.config = config;
        return f;
    }

    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        View v = inflater.inflate(R.layout.page_crop, container, false);
        img = (ImageView)v.findViewById(R.id.img);
        imgs = new ImageView[IDS.length];
        bitmaps = new Bitmap[IDS.length];
        for (int i = 0; i < IDS.length; i++){
            imgs[i] = (ImageView)v.findViewById(IDS[i]);
            bitmaps[i] = null;
        }
        return v;
    }

    public void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
    }

    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
    }

    public void onDestroyView() {
        super.onDestroyView();
        for (int i = 0; i < IDS.length; i++){
            imgs[i].setImageBitmap(null);
            if (bitmaps[i] != null){
                bitmaps[i].recycle();
            }
        }
    }

    public void onResume(){
        super.onResume(); 

        img.setImageResource(photoId);
        for (int i = 0; i < IDS.length; i++){
            if (bitmaps[i] != null){
                imgs[i].setImageBitmap(bitmaps[i]);
            }
        }

        new CropTask().execute();
    }


    class CropTask extends AsyncTask<Void, Void, Integer> { 

        protected void onPreExecute() {
        }

        protected Integer doInBackground(Void... params) {
            Log.d(TAG, "CropTask doInBackground");
            try {
                Bitmap src = ((BitmapDrawable)img.getDrawable()).getBitmap();
                if (src != null){
                    Log.d(TAG, "crop start");
                    for (int i = 0; i < IDS.length; i++){
                        if (bitmaps[i] == null){
                            Log.d(TAG, "crop bitmap " + i + " w=" + WIDTHS[i] + " h=" + HEIGHTS[i]);
                            bitmaps[i] = TClip.crop(config, src, WIDTHS[i], HEIGHTS[i]);
                            Log.d(TAG, "crop bitmap " + i + " done");
                        }
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
            return 0;
        }

        protected void onPostExecute(Integer result) {
            for (int i = 0; i < IDS.length; i++){
                if (bitmaps[i] != null && imgs[i] != null){
                    imgs[i].setImageBitmap(bitmaps[i]);
                }
            }
        }

    };

}
