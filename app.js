var log = console.log.bind(console);
var bigInt = require('big-integer');

var logForBigInts = function(bigInt){
  var string = bigInt.toString();
  return string.length * Math.log(10) + Math.log(parseFloat('0.' + string));
}

var getRSAStrength = function(modulus){
  var naturalLog = logForBigInts( bigInt(2).pow(modulus) )
  return Math.floor(Math.log2( Math.exp( Math.cbrt( (64/9) * naturalLog) 
    * Math.pow( Math.log( naturalLog ), 2/3) ) ))
}

var getECCStrength = function(pSize){
  return Math.log2(Math.pow(Math.pow(2, pSize), 0.5))
}

console.log("RSA  => "+getECCStrength(160));
console.log("RSA  => "+getECCStrength(224));
console.log("RSA  => "+getECCStrength(256));
console.log("RSA  => "+getECCStrength(368));
console.log("RSA  => "+getECCStrength(512));
//console.log("ECC   => "+getECCStrength(224));