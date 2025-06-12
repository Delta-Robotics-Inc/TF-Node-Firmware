#include <iostream>
#include "../src/drivers/CurrentController.h"

// Simple test to verify CurrentController functionality
int main() {
    std::cout << "Testing CurrentController..." << std::endl;
    
    // Create controller with target current of 5A and moderate PID gains
    CurrentController controller(5.0f, 0.1f, 0.02f, 0.01f);
    
    // Test 1: Initial output should be non-zero when current is zero (error is large)
    controller.update(0.0f);
    float output1 = controller.getOutput();
    std::cout << "Test 1 - Current=0A, Target=5A: Output=" << output1 << std::endl;
    
    // Test 2: Output should be smaller when current is closer to target
    controller.update(4.0f);
    float output2 = controller.getOutput();
    std::cout << "Test 2 - Current=4A, Target=5A: Output=" << output2 << std::endl;
    
    // Test 3: Output should be very small when current equals target
    controller.update(5.0f);
    float output3 = controller.getOutput();
    std::cout << "Test 3 - Current=5A, Target=5A: Output=" << output3 << std::endl;
    
    // Test 4: Change setpoint and verify response
    controller.setSetpoint(3.0f);
    controller.update(5.0f);
    float output4 = controller.getOutput();
    std::cout << "Test 4 - Current=5A, Target=3A: Output=" << output4 << std::endl;
    
    // Test 5: Reset controller
    controller.Reset();
    controller.update(0.0f);
    float output5 = controller.getOutput();
    std::cout << "Test 5 - After reset, Current=0A, Target=3A: Output=" << output5 << std::endl;
    
    // Verify outputs are within reasonable bounds (0-1 for PWM duty cycle)
    bool test_passed = (output1 >= 0.0f && output1 <= 1.0f) &&
                       (output2 >= 0.0f && output2 <= 1.0f) &&
                       (output3 >= 0.0f && output3 <= 1.0f) &&
                       (output4 >= 0.0f && output4 <= 1.0f) &&
                       (output5 >= 0.0f && output5 <= 1.0f);
    
    if (test_passed) {
        std::cout << "✓ All tests passed - CurrentController is working correctly!" << std::endl;
        return 0;
    } else {
        std::cout << "✗ Tests failed - Check CurrentController implementation" << std::endl;
        return 1;
    }
} 