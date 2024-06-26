module top(pi0, pi1, pi2, pi3, pi4, pi5, pi6, pi7, pi8, po0, po1, po2, po3, po4);
  input pi0, pi1, pi2, pi3, pi4, pi5, pi6, pi7, pi8;
  output po0, po1, po2, po3, po4;
  wire n10, n11, n12, n13, n14, n16, n17, n18, n19, n20, n21, n22, n23, n25, n26, n27, n28,
  n29, n30, n31, n32, n33, n34, n36, n37, n38, n39, n40, n41, n42, n43, n44, n45, n46, n47,
  n49, n50, n51, n52, n53, n54, n55, n56;
  assign n10 = ~pi0 & pi4;
  assign n11 = pi0 & ~pi4;
  assign n12 = ~n10 & ~n11;
  assign n13 = pi8 & n12;
  assign n14 = ~pi8 & ~n12;
  assign po0 = n13 | n14;
  assign n16 = ~pi1 & pi5;
  assign n17 = pi1 & ~pi5;
  assign n18 = ~n16 & ~n17;
  assign n19 = pi0 & pi4;
  assign n20 = pi8 & ~n12;
  assign n21 = ~n19 & ~n20;
  assign n22 = n18 & ~n21;
  assign n23 = ~n18 & n21;
  assign po1 = n22 | n23;
  assign n25 = ~pi2 & pi6;
  assign n26 = pi2 & ~pi6;
  assign n27 = ~n25 & ~n26;
  assign n30 = ~n18 & n20;
  assign n28 = ~n18 & n19;
  assign n29 = pi1 & pi5;
  assign n31 = ~n28 & ~n29;
  assign n32 = ~n30 & n31;
  assign n33 = n27 & ~n32;
  assign n34 = ~n27 & n32;
  assign po2 = n33 | n34;
  assign n36 = ~pi3 & pi7;
  assign n37 = pi3 & ~pi7;
  assign n38 = ~n36 & ~n37;
  assign n42 = ~n27 & n30;
  assign n40 = ~n27 & n28;
  assign n39 = ~n27 & n29;
  assign n41 = pi2 & pi6;
  assign n43 = ~n39 & ~n41;
  assign n44 = ~n40 & n43;
  assign n45 = ~n42 & n44;
  assign n46 = n38 & ~n45;
  assign n47 = ~n38 & n45;
  assign po3 = n46 | n47;
  assign n53 = ~n38 & n42;
  assign n51 = ~n38 & n40;
  assign n50 = ~n38 & n39;
  assign n49 = ~n38 & n41;
  assign n52 = pi3 & pi7;
  assign n54 = ~n49 & ~n52;
  assign n55 = ~n50 & n54;
  assign n56 = ~n51 & n55;
  assign po4 = n53 | ~n56;
endmodule
