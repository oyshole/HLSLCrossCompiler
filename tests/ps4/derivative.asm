//
// Generated by Microsoft (R) HLSL Shader Compiler 9.30.9200.20499
//
//
///
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// TEXCOORD                 0   xyzw        0     NONE   float   xyzw
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_Target                0   xyzw        0   TARGET   float   xyzw
//
ps_4_0
dcl_input_ps linear v0.xyzw
dcl_output o0.xyzw
dcl_temps 2
deriv_rtx r0.xyzw, v0.xyzw
deriv_rty r1.xyzw, v0.xyzw
mul o0.xyzw, r0.xyzw, r1.xyzw
ret 
// Approximately 4 instruction slots used