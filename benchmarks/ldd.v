// Benchmark "top" written by ABC on Tue Oct 24 16:48:13 2017

module top ( 
    pi0, pi1, pi2, pi3, pi4, pi5, pi6, pi7, pi8,
    po00, po01, po02, po03, po04, po05, po06, po07, po08, po09, po10, po11,
    po12, po13, po14, po15, po16, po17, po18 );
  input  pi0, pi1, pi2, pi3, pi4, pi5, pi6, pi7, pi8;
  output po00, po01, po02, po03, po04, po05, po06, po07, po08, po09, po10,
    po11, po12, po13, po14, po15, po16, po17, po18;
  wire n29, n30, n31, n32, n34, n35, n37, n38, n39, n40, n41, n42, n43, n44,
    n45, n46, n47, n48, n49, n50, n51, n52, n53, n55, n56, n58, n59, n60,
    n61, n62, n63, n64, n65, n66, n67, n68, n69, n70, n71, n72, n74, n76,
    n78, n79, n81, n82, n83, n84, n85, n86, n87, n88, n90, n91, n92, n93,
    n95, n96, n98, n99, n101, n102, n103, n104, n105, n106, n107, n108,
    n109, n111, n112, n113, n115, n116, n117, n118, n119, n120, n121, n122,
    n123, n124, n125, n127, n128, n129, n130, n131, n132, n133, n134, n136,
    n137, n138;
  assign n29 = ~pi0 & pi2;
  assign n30 = ~pi1 & n29;
  assign n31 = pi8 & n30;
  assign n32 = ~pi8 & n30;
  assign po00 = n31 | n32;
  assign n34 = pi0 & pi2;
  assign n35 = ~pi1 & n34;
  assign po07 = pi8 & n35;
  assign n37 = ~pi1 & ~pi2;
  assign n38 = ~pi4 & n37;
  assign n39 = ~pi3 & n38;
  assign n40 = pi5 & n39;
  assign n41 = ~pi0 & n40;
  assign n42 = ~pi0 & ~pi1;
  assign n43 = ~pi3 & n42;
  assign n44 = ~pi2 & n43;
  assign n45 = pi4 & n44;
  assign n46 = ~pi2 & ~pi3;
  assign n47 = ~pi5 & n46;
  assign n48 = ~pi4 & n47;
  assign n49 = pi6 & n48;
  assign n50 = ~pi1 & n49;
  assign n51 = ~pi0 & n50;
  assign n52 = ~po07 & ~n41;
  assign n53 = ~n45 & ~n51;
  assign po01 = ~n52 | ~n53;
  assign n55 = ~pi0 & ~pi2;
  assign n56 = ~pi1 & n55;
  assign po03 = pi3 & n56;
  assign n58 = ~pi3 & ~pi4;
  assign n59 = ~pi6 & n58;
  assign n60 = ~pi5 & n59;
  assign n61 = pi7 & n60;
  assign n62 = ~pi0 & n61;
  assign n63 = ~pi2 & n62;
  assign n64 = ~pi1 & n63;
  assign n65 = ~pi7 & n60;
  assign n66 = ~pi0 & n65;
  assign n67 = ~pi2 & n66;
  assign n68 = ~pi1 & n67;
  assign n69 = ~n64 & ~n68;
  assign n70 = ~n41 & n69;
  assign n71 = ~n45 & ~po03;
  assign n72 = ~n51 & n71;
  assign po02 = ~n70 | ~n72;
  assign n74 = pi1 & n55;
  assign po04 = pi8 & n74;
  assign n76 = pi1 & n29;
  assign po05 = pi8 & n76;
  assign n78 = pi0 & ~pi2;
  assign n79 = ~pi1 & n78;
  assign po06 = pi8 & n79;
  assign n81 = ~pi8 & n35;
  assign n82 = ~po05 & ~po06;
  assign n83 = ~po07 & ~n81;
  assign n84 = n82 & n83;
  assign n85 = ~n41 & ~n64;
  assign n86 = ~n45 & ~po04;
  assign n87 = ~n51 & n86;
  assign n88 = n85 & n87;
  assign po08 = ~n84 | ~n88;
  assign n90 = ~po07 & ~n64;
  assign n91 = n82 & n90;
  assign n92 = ~n41 & ~n51;
  assign n93 = n86 & n92;
  assign po09 = ~n91 | ~n93;
  assign n95 = ~n45 & ~po06;
  assign n96 = ~n51 & ~po04;
  assign po11 = ~n95 | ~n96;
  assign n98 = ~po07 & ~n45;
  assign n99 = ~n64 & ~po04;
  assign po12 = ~n98 | ~n99;
  assign n101 = pi1 & n78;
  assign n102 = pi8 & n101;
  assign n103 = ~pi8 & n101;
  assign n104 = ~pi8 & n79;
  assign n105 = ~n31 & ~n64;
  assign n106 = ~n102 & n105;
  assign n107 = ~po04 & ~po05;
  assign n108 = ~n51 & n107;
  assign n109 = n106 & n108;
  assign po17 = po06 | n104;
  assign n111 = ~n81 & ~n103;
  assign n112 = ~po07 & n111;
  assign n113 = ~po17 & n112;
  assign po13 = ~n109 | ~n113;
  assign n115 = ~pi8 & n74;
  assign n116 = ~pi8 & n76;
  assign n117 = ~n31 & ~n41;
  assign n118 = ~n102 & n117;
  assign n119 = ~po04 & ~n115;
  assign n120 = ~n45 & n119;
  assign n121 = n118 & n120;
  assign n122 = ~po05 & ~n116;
  assign n123 = ~po06 & ~n103;
  assign n124 = ~po07 & n123;
  assign n125 = n122 & n124;
  assign po14 = ~n121 | ~n125;
  assign n127 = ~po03 & ~n64;
  assign n128 = ~n102 & n127;
  assign n129 = ~po00 & ~n41;
  assign n130 = n128 & n129;
  assign n131 = ~po04 & ~n116;
  assign n132 = ~po05 & ~n81;
  assign n133 = ~po06 & n132;
  assign n134 = n131 & n133;
  assign po15 = ~n130 | ~n134;
  assign n136 = ~n115 & ~n116;
  assign n137 = ~po07 & n136;
  assign n138 = ~po04 & n132;
  assign po16 = ~n137 | ~n138;
  assign po18 = n102 | n103;
  assign po10 = po09;
endmodule


