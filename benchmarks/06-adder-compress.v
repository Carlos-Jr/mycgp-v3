// Benchmark "6-adder" written by ABC on Mon Mar 11 18:37:43 2024

module \6-adder  ( 
    pi0, pi1, pi2, pi3, pi4, pi5, pi6, pi7, pi8, pi9, pi10, pi11, pi12,
    po0, po1, po2, po3, po4, po5, po6  );
  input  pi0, pi1, pi2, pi3, pi4, pi5, pi6, pi7, pi8, pi9, pi10, pi11,
    pi12;
  output po0, po1, po2, po3, po4, po5, po6;
  wire n21, n22, n23, n24, n25, n26, n27,
    n28, n29, n30, n31, n32, n33, n34, n35,
    n36, n37, n38, n39, n40, n41, n42, n43,
    n45, n46, n47, n49, n50, n51, n53, n54,
    n55, n57, n58, n59, n61, n62, n63, n65,
    n66, n67;
  assign n21 = ~pi0 & ~pi5;
  assign n22 = pi0 & pi5;
  assign n23 = ~pi1 & ~pi6;
  assign n24 = pi1 & pi6;
  assign n25 = ~pi2 & ~pi7;
  assign n26 = pi2 & pi7;
  assign n27 = ~pi3 & ~pi8;
  assign n28 = pi3 & pi8;
  assign n29 = ~pi4 & ~pi9;
  assign n30 = ~pi11 & ~pi12;
  assign n31 = pi11 & pi12;
  assign n32 = ~pi10 & ~n31;
  assign n33 = ~n30 & ~n32;
  assign n34 = pi4 & pi9;
  assign n35 = ~n33 & ~n34;
  assign n36 = ~n29 & ~n35;
  assign n37 = ~n28 & ~n36;
  assign n38 = ~n27 & ~n37;
  assign n39 = ~n26 & ~n38;
  assign n40 = ~n25 & ~n39;
  assign n41 = ~n24 & ~n40;
  assign n42 = ~n23 & ~n41;
  assign n43 = ~n22 & ~n42;
  assign po0 = ~n21 & ~n43;
  assign n45 = ~n21 & ~n22;
  assign n46 = ~n42 & n45;
  assign n47 = n42 & ~n45;
  assign po1 = n46 | n47;
  assign n49 = ~n23 & ~n24;
  assign n50 = ~n40 & n49;
  assign n51 = n40 & ~n49;
  assign po2 = n50 | n51;
  assign n53 = ~n25 & ~n26;
  assign n54 = ~n38 & n53;
  assign n55 = n38 & ~n53;
  assign po3 = n54 | n55;
  assign n57 = ~n27 & ~n28;
  assign n58 = n36 & ~n57;
  assign n59 = ~n36 & n57;
  assign po4 = n58 | n59;
  assign n61 = ~n29 & ~n34;
  assign n62 = n33 & ~n61;
  assign n63 = ~n29 & n35;
  assign po5 = n62 | n63;
  assign n65 = ~n30 & ~n31;
  assign n66 = pi10 & ~n65;
  assign n67 = ~pi10 & n65;
  assign po6 = n66 | n67;
endmodule


