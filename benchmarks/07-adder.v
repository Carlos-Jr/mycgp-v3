// Benchmark "7-adder" written by ABC on Tue May 23 15:44:42 2017

module 7-adder ( 
    pi00, pi01, pi02, pi03, pi04, pi05, pi06, pi07, pi08, pi09, pi10, pi11,
    pi12, pi13, pi14,
    po0, po1, po2, po3, po4, po5, po6, po7 );
  input  pi00, pi01, pi02, pi03, pi04, pi05, pi06, pi07, pi08, pi09,
    pi10, pi11, pi12, pi13, pi14;
  output po0, po1, po2, po3, po4, po5, po6, po7;
  wire n24, n25, n26, n27, n28, n29, n30, n31, n32, n33, n34, n35, n36, n37,
    n38, n39, n40, n41, n42, n43, n44, n45, n46, n47, n48, n49, n50, n51,
    n52, n53, n54, n56, n57, n58, n59, n60, n61, n62, n63, n64, n65, n66,
    n67, n68, n69, n70, n71, n72, n73, n74, n76, n77, n78, n79, n80, n81,
    n82, n83, n85, n86, n87, n88, n89, n90, n91, n93, n94, n95, n97, n98,
    n99, n100, n101, n103, n104, n105, n107, n108, n109;
  assign n24 = pi00 & pi06;
  assign n25 = ~pi04 & ~pi10;
  assign n26 = ~pi13 & ~pi14;
  assign n27 = pi12 & ~n26;
  assign n28 = pi13 & pi14;
  assign n29 = ~n27 & ~n28;
  assign n30 = ~pi05 & ~pi11;
  assign n31 = ~n29 & ~n30;
  assign n32 = ~n25 & n31;
  assign n33 = ~pi03 & ~pi09;
  assign n34 = ~pi02 & ~pi08;
  assign n35 = ~n33 & ~n34;
  assign n36 = ~pi01 & ~pi07;
  assign n37 = ~pi00 & ~pi06;
  assign n38 = ~n36 & ~n37;
  assign n39 = n35 & n38;
  assign n40 = n32 & n39;
  assign n41 = pi05 & pi11;
  assign n42 = pi04 & pi10;
  assign n43 = ~n41 & ~n42;
  assign n44 = ~n25 & ~n43;
  assign n45 = n35 & n44;
  assign n46 = pi01 & pi07;
  assign n47 = pi03 & pi09;
  assign n48 = pi02 & pi08;
  assign n49 = ~n47 & ~n48;
  assign n50 = ~n34 & ~n49;
  assign n51 = ~n46 & ~n50;
  assign n52 = ~n45 & n51;
  assign n53 = n38 & ~n52;
  assign n54 = ~n24 & ~n40;
  assign po0 = n53 | ~n54;
  assign n56 = ~n24 & ~n37;
  assign n57 = n32 & n35;
  assign n58 = n52 & ~n57;
  assign n59 = ~n36 & ~n56;
  assign n60 = ~n58 & n59;
  assign n61 = ~n35 & ~n48;
  assign n62 = ~n46 & n61;
  assign n63 = ~n36 & ~n62;
  assign n64 = n56 & ~n63;
  assign n65 = ~pi12 & ~n28;
  assign n66 = ~n26 & ~n65;
  assign n67 = ~n41 & ~n66;
  assign n68 = ~n25 & ~n30;
  assign n69 = ~n67 & n68;
  assign n70 = ~n42 & ~n69;
  assign n71 = ~n46 & n49;
  assign n72 = n56 & n71;
  assign n73 = n70 & n72;
  assign n74 = ~n64 & ~n73;
  assign po1 = n60 | ~n74;
  assign n76 = ~n36 & ~n46;
  assign n77 = ~n32 & ~n44;
  assign n78 = n35 & ~n77;
  assign n79 = ~n50 & ~n76;
  assign n80 = ~n78 & n79;
  assign n81 = n49 & n70;
  assign n82 = ~n61 & n76;
  assign n83 = ~n81 & n82;
  assign po2 = ~n80 & ~n83;
  assign n85 = ~n34 & ~n48;
  assign n86 = ~n33 & ~n77;
  assign n87 = ~n47 & ~n86;
  assign n88 = ~n85 & ~n87;
  assign n89 = ~n47 & n70;
  assign n90 = ~n33 & ~n89;
  assign n91 = n85 & ~n90;
  assign po3 = n88 | n91;
  assign n93 = ~n33 & ~n47;
  assign n94 = ~n77 & ~n93;
  assign n95 = n70 & n93;
  assign po4 = n94 | n95;
  assign n97 = ~n31 & ~n41;
  assign n98 = ~n25 & ~n42;
  assign n99 = ~n97 & ~n98;
  assign n100 = ~n30 & ~n67;
  assign n101 = n98 & ~n100;
  assign po5 = n99 | n101;
  assign n103 = ~n30 & ~n41;
  assign n104 = ~n29 & ~n103;
  assign n105 = ~n66 & n103;
  assign po6 = n104 | n105;
  assign n107 = ~n26 & ~n28;
  assign n108 = pi12 & ~n107;
  assign n109 = ~pi12 & n107;
  assign po7 = n108 | n109;
endmodule


