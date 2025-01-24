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