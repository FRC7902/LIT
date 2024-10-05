package frc.robot;

import java.nio.charset.StandardCharsets;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.concurrent.locks.ReentrantLock;

import edu.wpi.first.hal.HAL;
import edu.wpi.first.hal.SerialPortJNI;
import edu.wpi.first.hal.FRCNetComm.tResourceType;
import edu.wpi.first.wpilibj.Timer;

public class PicoLEDModule implements AutoCloseable {
  private double lastReadTime;
  private final Thread readThread;
  private final AtomicBoolean threadRunning = new AtomicBoolean(true);
  int port = SerialPortJNI.serialInitializePort((byte)1);

  public static class RawColor {
    public RawColor(int r, int g, int b, int _ir) {
      red = r;
      green = g;
      blue = b;
      ir = _ir;
    }

    public RawColor() {
    }

    public int red;
    public int green;
    public int blue;
    public int ir;
  }

  private static class SingleCharSequence implements CharSequence {
    public byte[] data;

    @Override
    public int length() {
      return data.length;
    }

    @Override
    public char charAt(int index) {
      return (char)data[index];
    }

    @Override
    public CharSequence subSequence(int start, int end) {
      return new String(data, start, end, StandardCharsets.UTF_8);
    }

  }
  
  public void threadMain() {   
    // Using JNI for a non allocating read
    SerialPortJNI.serialSetBaudRate(port, 4800);
    SerialPortJNI.serialSetDataBits(port, (byte)8);
    SerialPortJNI.serialSetParity(port, (byte)0);
    SerialPortJNI.serialSetStopBits(port, (byte)10);

    SerialPortJNI.serialSetWriteBufferSize(port, 64);
    SerialPortJNI.serialSetWriteMode(port, (byte)1);
  }

  public void skibidi_sigma() {

//    HAL.report(tResourceType.kResourceType_SerialPort, 2);
    
    System.out.println("skibidi " + port);

    byte[] command = {(byte) 3, (byte) 255, (byte) 255, (byte) 255};

    SerialPortJNI.serialWrite(port, command, 4);
    
    //SerialPortJNI.serialClose(port);
  }
  

  public PicoLEDModule() {
    readThread = new Thread(this::threadMain);
    readThread.setName("PicoColourController");
    readThread.start();
  }


  @Override
  public void close() throws Exception {
    threadRunning.set(false);
    readThread.join();
  }
}