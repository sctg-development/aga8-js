/**
 * @copyright Copyright (c) 2024-2025 Ronan LE MEILLAT
 * @license AGPL-3.0-or-later
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
export type ScientificNotationNumber = {

    mantissa: number; // Mantissa
    exponent: number; // Exponent rounded to the nearest multiple of 3
  };

export class ScientificNotation {
    /**
     * Convert a number to scientific notation
     * @param value - Value to convert
     * @param precision - Number of significant digits (optinal default: 3)
     * @returns {number, number} - Mantissa and exponent
     */
    public static toScientificNotation(value: number, precision?: number): ScientificNotationNumber {
      if (precision === undefined) {
        precision = 3;
      } else if (precision < 1) {
        precision = 0;
      } else {
        precision = Math.floor(precision - 1);
      }
  
      if (value === 0) {
        return { mantissa: 0, exponent: 0 };
      }
  
      // Compute the exponent
      const exp = Math.floor(Math.log10(Math.abs(value)));
  
      // round to the nearest multiple of 3
      const normalizedExp = Math.floor(exp / 3) * 3;
  
      // Compute the mantissa
      let mantissa = value / Math.pow(10, normalizedExp);
      // Round mantissa to precision decimal places
      mantissa = Math.round(mantissa * 10 ** precision) / 10 ** precision;
  
      return { mantissa, exponent: normalizedExp };
    }
  
    /**
     * Convert a number to scientific notation string
     * @param value - Value to convert
     * @param precision - Number of significant digits (optinal default: 3)
     * @returns {string} - Value in scientific notation
     */
    public static toScientificNotationString(value: number, precision ?: number): string {
      const { mantissa, exponent } = this.toScientificNotation(value, precision);
      return `${mantissa}e${exponent}`;
    }
  
    /**
     * Convert a number to scientific notation latex string
     * @param value - Value to convert
     * @param precision - Number of significant digits (optinal default: 3)
     * @returns {string} - Value in scientific notation latex
     */
    public static toScientificNotationLatex(value: number, precision ?: number): string {
      const { mantissa, exponent } = this.toScientificNotation(value, precision);
      return `${mantissa} \\times 10^{${exponent}}`;
    }
  
    /**
     * Convert a number to scientific notation MathML string
     * @param value - Value to convert
     * @param precision - Number of significant digits (optinal default: 3)
     * @returns {string} - Value in scientific notation MathML
     */
    public static toScientificNotationMathML(value: number, precision ?: number): string {
      const { mantissa, exponent } = this.toScientificNotation(value, precision);
      return `${mantissa} <msup><mn>10</mn><mn>${exponent}</mn></msup>`;
    }
  
    /**
     * Convert a number to scientific notation HTML string
     * @param value - Value to convert
     * @param precision - Number of significant digits
     * @returns {string} - Value in scientific notation HTML
     */
    public static toScientificNotationHTML(value: number, precision ?: number): string {
      const { mantissa, exponent } = this.toScientificNotation(value, precision);
      return `${mantissa} × 10<sup>${exponent}</sup>`;
    }
  }