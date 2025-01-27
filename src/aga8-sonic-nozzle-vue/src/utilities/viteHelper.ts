/**
 * @copyright Copyright (c) 2024 Ronan LE MEILLAT
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

/**
 * Generates a version 4 UUID.
 *
 * @return {string} The generated UUID.
 */
export function generateUUID(): string{
  return 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g, function(c) {
      const r = Math.random() * 16 | 0,
          v = c === 'x' ? r : (r & 0x3 | 0x8);
      return v.toString(16);
  });
}

/**
 * Generates a base64-encoded version 4 UUID.
 *
 * @return {string} The generated base64-encoded UUID.
 */
export function generateUUIDBase64Encoded(): string {
  //equivalent of bash command: uuidgen | base64
  return btoa(generateUUID());
}

/**
 * Generates unique id compatible with an HTML id and based on a v4 UUID.
 *
 * @return {string} The generated ID.
 */
export function generateUniqueId(): string {
  return "ID"+generateUUID().replace(/-/g, '');
}