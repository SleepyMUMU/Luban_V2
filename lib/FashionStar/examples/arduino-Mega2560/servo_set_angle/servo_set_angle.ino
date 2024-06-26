/* 
 * 设置舵机的角度(单圈模式)
 * 提示: 拓展板上电之后, 记得按下Arduino的RESET按键
 * --------------------------
 * 作者: 深圳市华馨京科技有限公司
 * 网站：https://fashionrobo.com/
 * 更新时间: 2023/03/13
 */

#include "FashionStar_UartServoProtocol.h"
#include "FashionStar_UartServo.h" // Fashion Star串口总线舵机的依赖

// 串口总线舵机配置
#define USERVO_BAUDRATE (uint32_t)115200 // 波特率

// 调试串口的配置
#define DEBUG_SERIAL Serial
#define DEBUG_SERIAL_BAUDRATE (uint32_t)115200

//多串口版本主要区别在于：串口舵机管理器&舵机挂载在串口上，2个部分//
// 串口1舵机管理器
FSUS_Protocol protocol_ch1(&Serial1, USERVO_BAUDRATE);
// 串口2舵机管理器
FSUS_Protocol protocol_ch2(&Serial2, USERVO_BAUDRATE);

// 舵机 #0 挂载在串口1上
FSUS_Servo uservo_0(0, &protocol_ch1); // 创建舵机
// 舵机 #1 挂载在串口2上
FSUS_Servo uservo_1(1, &protocol_ch2); // 创建舵机
///////////////////*请以上面串口1,2范例为标准*////////////////


/* 等待并报告当前的角度*/
void waitAndReport(){
    uservo_0.wait();          // 等待舵机旋转到目标角度
    DEBUG_SERIAL.println("Real Angle = " + String(uservo_0.curRawAngle, 1) + " Target Angle = "+String(uservo_0.targetRawAngle, 1));
    delay(2000); // 暂停2s
    
    uservo_1.wait();          // 等待舵机旋转到目标角度
    DEBUG_SERIAL.println("Real Angle = " + String(uservo_1.curRawAngle, 1) + " Target Angle = "+String(uservo_1.targetRawAngle, 1));
    delay(2000); // 暂停2s
}

void setup(){
    // 串口总线舵机 #0 初始化
    uservo_0.init(); 
    // 串口总线舵机 #1 初始化
    uservo_1.init(); 

    // 打印例程信息
    DEBUG_SERIAL.begin(DEBUG_SERIAL_BAUDRATE); // 初始化软串口的波特率
    DEBUG_SERIAL.println("Set Servo Angle");
}

void loop(){
  
    DEBUG_SERIAL.println("Set Angle = 90°");
    uservo_0.setRawAngle(90.0);  // 设置舵机的角度
    DEBUG_SERIAL.println("Set Angle = 90°");
    uservo_1.setRawAngle(90.0);  // 设置舵机的角度
    //waitAndReport();
    delay(2000);
    
    DEBUG_SERIAL.println("Set Angle = -90°");
    uservo_0.setRawAngle(-90);
    DEBUG_SERIAL.println("Set Angle = -90°");
    uservo_1.setRawAngle(-90);
    //waitAndReport();
    delay(2000);
}
