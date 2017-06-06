package com.example.dexheimere.hw19;

import android.Manifest;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.drawable.BitmapDrawable;
import android.hardware.Camera;
import android.hardware.usb.UsbDeviceConnection;
import android.hardware.usb.UsbManager;
import android.net.Uri;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Base64;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.TextureView;
import android.view.View;
import android.view.WindowManager;
import android.widget.ImageView;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.TextView;
import android.widget.Button;
import android.widget.ScrollView;
import android.graphics.SurfaceTexture;

import com.hoho.android.usbserial.driver.CdcAcmSerialDriver;
import com.hoho.android.usbserial.driver.ProbeTable;
import com.hoho.android.usbserial.driver.UsbSerialDriver;
import com.hoho.android.usbserial.driver.UsbSerialPort;
import com.hoho.android.usbserial.driver.UsbSerialProber;
import com.hoho.android.usbserial.util.SerialInputOutputManager;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.List;
import java.util.Scanner;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

import static android.graphics.Color.blue;
import static android.graphics.Color.green;
import static android.graphics.Color.red;
import static android.graphics.Color.rgb;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import static java.lang.Math.abs;

public class MainActivity extends AppCompatActivity implements TextureView.SurfaceTextureListener {

    SeekBar myControl;
    //TextView myTextView;
    //Button button;
    //TextView myTextView2;
    //ScrollView myScrollView;
    //TextView myTextView3;

    private Camera mCamera;
    private TextureView mTextureView;
    private SurfaceView mSurfaceView;
    private SurfaceHolder mSurfaceHolder;
    private Bitmap bmp = Bitmap.createBitmap(640, 480, Bitmap.Config.ARGB_8888);
    private Canvas canvas = new Canvas(bmp);
    private Paint paint1 = new Paint();
    private TextView mTextView4;

    //private Bitmap map_bmp = BitmapFactory.decodeFile("/storage/emulated/0/Pictures/techCup2017.png");
    private Bitmap map_bmp = Bitmap.createBitmap(800, 600, Bitmap.Config.ARGB_8888);
    private Canvas canvas_map = new Canvas(map_bmp);
    private ImageView imageView;

    static long prevtime = 0; // for FPS calculation

    private UsbManager manager;
    private UsbSerialPort sPort;
    private final ExecutorService mExecutor = Executors.newSingleThreadExecutor();
    private SerialInputOutputManager mSerialIoManager;

    private int x;
    private int y;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON); // keeps the screen from turning off

        myControl = (SeekBar) findViewById(R.id.seek1);
        //myTextView = (TextView) findViewById(R.id.textView01);
        //myTextView.setText("Enter whatever you Like!");
        //myTextView2 = (TextView) findViewById(R.id.textView02);
        //myScrollView = (ScrollView) findViewById(R.id.ScrollView01);
        //myTextView3 = (TextView) findViewById(R.id.textView03);
        //button = (Button) findViewById(R.id.button1);
        mTextView4 = (TextView) findViewById(R.id.cameraStatus);

        // uncomment this
        //map_bmp = BitmapFactory.decodeFile("/storage/emulated/0/Pictures/map.png");
        //imageView = (ImageView) findViewById(R.id.imageViewMap);

        /*button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                myTextView2.setText("value on click is " + myControl.getProgress());
                String sendString = String.valueOf(myControl.getProgress()) + '\n';
                try {
                    sPort.write(sendString.getBytes(), 10); // 10 is the timeout
                } catch (IOException e) {
                }
            }
        });*/

        setMyControlListener();
        manager = (UsbManager) getSystemService(Context.USB_SERVICE);

        // see if the app has permission to use the camera
        //ActivityCompat.requestPermissions(MainActivity.this, new String[]{Manifest.permission.CAMERA}, 1);
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.CAMERA) == PackageManager.PERMISSION_GRANTED) {
            mSurfaceView = (SurfaceView) findViewById(R.id.surfaceview);
            mSurfaceHolder = mSurfaceView.getHolder();

            mTextureView = (TextureView) findViewById(R.id.textureview);
            mTextureView.setSurfaceTextureListener(this);

            // set the paintbrush for writing text on the image
            paint1.setColor(0xffff0000); // red
            paint1.setTextSize(24);

            mTextView4.setText("started camera");
        } else {
            mTextView4.setText("no camera permissions");
        }

    }

    private void setMyControlListener() {
        myControl.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {

            int progressChanged = 0;

            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                progressChanged = progress;
                //myTextView.setText("The value is: " + progress);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });
    }

    private final SerialInputOutputManager.Listener mListener =
            new SerialInputOutputManager.Listener() {
                @Override
                public void onRunError(Exception e) {

                }

                @Override
                public void onNewData(final byte[] data) {
                    MainActivity.this.runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            MainActivity.this.updateReceivedData(data);
                        }
                    });
                }
            };

    @Override
    protected void onPause() {
        super.onPause();
        stopIoManager();
        if (sPort != null) {
            try {
                sPort.close();
            } catch (IOException e) {
            }
            sPort = null;
        }
        finish();
    }

    @Override
    protected void onResume() {
        super.onResume();

        ProbeTable customTable = new ProbeTable();
        customTable.addProduct(0x04D8, 0x000A, CdcAcmSerialDriver.class);
        UsbSerialProber prober = new UsbSerialProber(customTable);

        final List<UsbSerialDriver> availableDrivers = prober.findAllDrivers(manager);

        if (availableDrivers.isEmpty()) {
            //check
            return;
        }

        UsbSerialDriver driver = availableDrivers.get(0);
        sPort = driver.getPorts().get(0);

        if (sPort == null) {
            //check
        } else {
            final UsbManager usbManager = (UsbManager) getSystemService(Context.USB_SERVICE);
            UsbDeviceConnection connection = usbManager.openDevice(driver.getDevice());
            if (connection == null) {
                //check
                PendingIntent pi = PendingIntent.getBroadcast(this, 0, new Intent("com.android.example.USB_PERMISSION"), 0);
                usbManager.requestPermission(driver.getDevice(), pi);
                return;
            }

            try {
                sPort.open(connection);
                sPort.setParameters(9600, 8, UsbSerialPort.STOPBITS_1, UsbSerialPort.PARITY_NONE);

            } catch (IOException e) {
                //check
                try {
                    sPort.close();
                } catch (IOException e1) {
                }
                sPort = null;
                return;
            }
        }
        onDeviceStateChange();
    }

    private void stopIoManager() {
        if (mSerialIoManager != null) {
            mSerialIoManager.stop();
            mSerialIoManager = null;
        }
    }

    private void startIoManager() {
        if (sPort != null) {
            mSerialIoManager = new SerialInputOutputManager(sPort, mListener);
            mExecutor.submit(mSerialIoManager);
        }
    }

    private void onDeviceStateChange() {
        stopIoManager();
        startIoManager();
    }

    private void updateReceivedData(byte[] data) {
        //do something with received data

        //for displaying:
        String rxString = null;
        try {
            rxString = new String(data, "UTF-8"); // put the data you got into a string
            //Scanner sc = new Scanner(rxString);
            //x = sc.nextInt();
            //y = sc.nextInt();

            // draw dot on map
            /*Bitmap tempBitmap = Bitmap.createBitmap(map_bmp.getWidth(), map_bmp.getHeight(), Bitmap.Config.RGB_565);
            Canvas tempCanvas = new Canvas(tempBitmap);
            tempCanvas.drawBitmap(map_bmp, 0, 0, null);
            tempCanvas.drawCircle(x, y, 2, paint1);
            imageView.setImageDrawable(new BitmapDrawable(getResources(), tempBitmap));*/
            //myTextView3.append(rxString);
            //myScrollView.fullScroll(View.FOCUS_DOWN);
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }

    }

    public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
        mCamera = Camera.open();
        Camera.Parameters parameters = mCamera.getParameters();
        parameters.setPreviewSize(640, 480);
        parameters.setFocusMode(Camera.Parameters.FOCUS_MODE_INFINITY); // no autofocusing
        parameters.setAutoExposureLock(true); // keep the white balance constant
        mCamera.setParameters(parameters);
        mCamera.setDisplayOrientation(90); // rotate to portrait mode

        try {
            mCamera.setPreviewTexture(surface);
            mCamera.startPreview();
        } catch (IOException ioe) {
            // Something bad happened
        }
    }

    public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {
        // Ignored, Camera does all the work for us
    }

    public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
        mCamera.stopPreview();
        mCamera.release();
        return true;
    }

    // the important function
    public void onSurfaceTextureUpdated(SurfaceTexture surface) {
        // every time there is a new Camera preview frame
        mTextureView.getBitmap(bmp);
        SeekBar seek1 = (SeekBar) findViewById(R.id.seek1);

        // min RGB [138, 135, 86]
        // max RGB [231, 232, 233]
        // give constant upper bound tolerance, slider bar lower bound

        int com_x = 0;
        int com_y = 0;
        long com_sum = 0;
        long num_com = 0;
        int sum_y = 0;
        final Canvas c = mSurfaceHolder.lockCanvas();
        if (c != null) {
            int thresh = seek1.getProgress(); // comparison value
            int[] pixels = new int[bmp.getWidth()]; // pixels[] is the RGBA data
            //int startY = 200; // which row in the bitmap to analyze to read
            for (int j = 0; j < bmp.getHeight(); j += 5) {
                bmp.getPixels(pixels, 0, bmp.getWidth(), 0, j, bmp.getWidth(), 1);

                int sum_x = 0;
                int num_pixels = 0;
                // in the row, see if there is more green than red
                for (int i = 0; i < bmp.getWidth(); i+=1) {
                    /*if (((red(pixels[i]) - green(pixels[i])) < thresh) && ((green(pixels[i]) - blue(pixels[i])) < thresh)
                            && ((red(pixels[i]) - green(pixels[i])) > -thresh) && ((green(pixels[i]) - blue(pixels[i])) > -thresh)
                            && ((blue(pixels[i]) - red(pixels[i])) > -thresh) && ((blue(pixels[i]) - red(pixels[i])) < thresh)) {*/
                    /*if ( (red(pixels[i]) > 138 + thresh) && (green(pixels[i]) > 135 + thresh) && (blue(pixels[i]) > 86+thresh)
                            && (red(pixels[i]) < 231+thresh) && (green(pixels[i]) < 232+thresh) && (blue(pixels[i]) < 233+thresh)
                            && (red(pixels[i]) + green(pixels[i]) + blue(pixels[i]) < 730 ) ) {*/
                    if ( red(pixels[i]) - green(pixels[i]) > thresh ) {
                        pixels[i] = rgb(255, 0, 0); // over write the pixel with pure green
                        sum_x += i;
                        ++num_pixels;
                    }
                }
                // update center of mass
                if (num_pixels == 0) {
                    continue;
                }
                com_sum += sum_x / num_pixels;
                ++num_com;
                sum_y += j;

                //com_x = sum_x/num_pixels;
                //canvas.drawCircle(com_x, j, 5, paint1); // x position, y position, diameter, color
                //pixels[com] = rgb(255,0,0);
                // update the row
                bmp.setPixels(pixels, 0, bmp.getWidth(), 0, j, bmp.getWidth(), 1);
            }
            if (num_com == 0) {
                com_x = bmp.getWidth()/2;
                com_y = bmp.getHeight()/2;
            }
            else {
                com_x = (int) (com_sum / num_com);
                com_y = (int) (sum_y / num_com);
                canvas.drawCircle(com_x, com_y, 5, paint1);
            }

        }

        double x_weight = 2;
        double y_weight = 0.01;
        int motor_offset = (int) (x_weight*(com_x-bmp.getWidth()/2) * y_weight*com_y );
        int neutral_power = 3000;  // pwm are 0 to 8000
        int pwm1 = neutral_power + motor_offset;
        int pwm2 = neutral_power - motor_offset;
        
        // write the pos as text
        //canvas.drawText("pos = " + pos, 10, 200, paint1);
        c.drawBitmap(bmp, 0, 0, null);
        mSurfaceHolder.unlockCanvasAndPost(c);

        // draw dot on map
        /*Bitmap tempBitmap = Bitmap.createBitmap(map_bmp.getWidth(), map_bmp.getHeight(), Bitmap.Config.RGB_565);
        Canvas tempCanvas = new Canvas(tempBitmap);
        tempCanvas.drawBitmap(map_bmp, 0, 0, null);
        tempCanvas.drawCircle(com_x, 300, 5, paint1);
        imageView.setImageDrawable(new BitmapDrawable(getResources(), tempBitmap));*/

        // calculate the FPS to see how fast the code is running
        long nowtime = System.currentTimeMillis();
        long diff = nowtime - prevtime;
        mTextView4.setText("FPS " + 1000 / diff);
        prevtime = nowtime;

        String sendString =  String.valueOf(pwm1) + ' ' + String.valueOf(pwm2) + '\n';
        try {
            sPort.write(sendString.getBytes(), 100); // 10 is the timeout
        } catch (IOException e) {
        }

    }

}

