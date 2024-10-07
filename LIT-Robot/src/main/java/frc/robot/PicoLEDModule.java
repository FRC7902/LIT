package frc.robot;

import java.util.concurrent.atomic.AtomicBoolean;
import edu.wpi.first.hal.SerialPortJNI;
import edu.wpi.first.wpilibj.DigitalOutput;
import frc.robot.Constants.LightConstants;

public class PicoLEDModule implements AutoCloseable {
  private final Thread readThread;
  private final AtomicBoolean threadRunning = new AtomicBoolean(true);
  private int port = SerialPortJNI.serialInitializePort((byte)1);
  private DigitalOutput interrupt = new DigitalOutput(LightConstants.dioPin);

  public void threadMain() {   
    SerialPortJNI.serialSetBaudRate(port, LightConstants.baudRate);
    SerialPortJNI.serialSetDataBits(port, (byte)8);
    SerialPortJNI.serialSetParity(port, (byte)0);
    SerialPortJNI.serialSetStopBits(port, (byte)10);
    SerialPortJNI.serialSetWriteBufferSize(port, LightConstants.bufferSize);
    SerialPortJNI.serialSetWriteMode(port, (byte)1);
  }

  public void interrupt(boolean pinValue) {
    interrupt.set(pinValue);
  }

  public void skibidi_sigma(int brightness, int mode, int r, int g, int b) {
    final byte[] command = {(byte) brightness, (byte) mode, (byte) r, (byte) g, (byte) b};
    SerialPortJNI.serialInitializePort((byte)1);
  //HAL.report(tResourceType.kResourceType_SerialPort, 2);
  //System.out.println("skibidi " + port);
    SerialPortJNI.serialWrite(port, command, 5);
    SerialPortJNI.serialClose(port);
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