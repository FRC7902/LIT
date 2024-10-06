// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot.commands;

import frc.robot.subsystems.LEDSubsystem;
import edu.wpi.first.wpilibj2.command.Command;
import frc.robot.PicoLEDModule;

/** An example command that uses an example subsystem. */
public class sendInterrupt extends Command {
  private PicoLEDModule m_pico;
  private int mode, r, g, b;
  private boolean interruptValue;

  @SuppressWarnings({"PMD.UnusedPrivateField", "PMD.SingularField"})
  


  /**
   * Creates a new ExampleCommand.
   *
   * @param subsystem The subsystem used by this command.
   */
  public sendInterrupt(PicoLEDModule pico, boolean pinValue) {
    // Use addRequirements() here to declare subsystem dependencies.
    m_pico = pico;
    interruptValue = pinValue;
  }

  // Called when the command is initially scheduled.
  @Override
  public void initialize() {
    m_pico.interrupt(interruptValue);

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
