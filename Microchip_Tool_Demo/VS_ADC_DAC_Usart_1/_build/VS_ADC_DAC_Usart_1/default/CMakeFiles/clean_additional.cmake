# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "C:\\Temp\\VS_ADC_DAC_Usart_1\\out\\VS_ADC_DAC_Usart_1\\default.eep"
  "C:\\Temp\\VS_ADC_DAC_Usart_1\\out\\VS_ADC_DAC_Usart_1\\default.hex"
  "C:\\Temp\\VS_ADC_DAC_Usart_1\\out\\VS_ADC_DAC_Usart_1\\default.lss"
  "C:\\Temp\\VS_ADC_DAC_Usart_1\\out\\VS_ADC_DAC_Usart_1\\default.srec"
  "C:\\Temp\\VS_ADC_DAC_Usart_1\\out\\VS_ADC_DAC_Usart_1\\default.usersignatures"
  )
endif()
