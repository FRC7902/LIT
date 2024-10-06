// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot.commands;

import frc.robot.subsystems.LEDSubsystem;
import edu.wpi.first.wpilibj2.command.Command;
import frc.robot.PicoLEDModule;

public class sendMessage extends Command {
  private PicoLEDModule m_pico;
  private int mode, r, g, b;

  @SuppressWarnings({"PMD.UnusedPrivateField", "PMD.SingularField"})
  
  /**
   * Creates a new ExampleCommand.
   *
   * @param subsystem The subsystem used by this command.
   */

  public sendMessage(PicoLEDModule pico) {
    // Use addRequirements() here to declare subsystem dependencies.
    m_pico = pico;
  }

  // Called when the command is initially scheduled.
  @Override
  public void initialize() {
    //System.out.println("button received");
    int mode = 1, 
        r = 0, 
        g = 255, 
        b = 255;
    m_pico.skibidi_sigma(mode, r, g, b);
  }

  // Called every time the scheduler runs while the command is scheduled.
  @Override
  public void execute() {}

  // Called once the command ends or is interrupted.
  @Override
  public void end(boolean interrupted) {}

  // Returns true when the command should end.
  @Override
  public boolean isFinished() {
    return false;
  }
}
