// Benchmark "decod" written by ABC on Sun Apr 22 21:43:02 2018

module decod ( 
    pi0, pi1, pi2, pi3, pi4,
    po0, po1, po2, po3, po4, po5, po6, po7, po8, po9, po10, po11,
    po12, po13, po14, po15 );
  input  pi0, pi1, pi2, pi3, pi4;
  output po0, po1, po2, po3, po4, po5, po6, po7, po8, po9, po10,
    po11, po12, po13, po14, po15;
  wire n22, n23, n24, n26, n27, n29, n30, n32, n33, n35, n37, n39, n41, n43;
  assign n22 = pi0 & pi4;
  assign n23 = pi2 & pi3;
  assign n24 = pi1 & n23;
  assign po0 = n22 & n24;
  assign n26 = pi2 & ~pi3;
  assign n27 = pi1 & n26;
  assign po1 = n22 & n27;
  assign n29 = ~pi2 & pi3;
  assign n30 = pi1 & n29;
  assign po2 = n22 & n30;
  assign n32 = ~pi2 & ~pi3;
  assign n33 = pi1 & n32;
  assign po3 = n22 & n33;
  assign n35 = ~pi1 & n23;
  assign po4 = n22 & n35;
  assign n37 = ~pi1 & n26;
  assign po5 = n22 & n37;
  assign n39 = ~pi1 & n29;
  assign po6 = n22 & n39;
  assign n41 = ~pi1 & n32;
  assign po7 = n22 & n41;
  assign n43 = ~pi0 & pi4;
  assign po8 = n24 & n43;
  assign po9 = n27 & n43;
  assign po10 = n30 & n43;
  assign po11 = n33 & n43;
  assign po12 = n35 & n43;
  assign po13 = n37 & n43;
  assign po14 = n39 & n43;
  assign po15 = n41 & n43;
endmodule


