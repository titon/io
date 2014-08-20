<?hh // strict
/**
 * @copyright   2010-2013, The Titon Project
 * @license     http://opensource.org/licenses/bsd-license.php
 * @link        http://titon.io
 */

namespace Titon\Io\Reader;

use Titon\Io\Exception\ReadErrorException;

/**
 * A file reader that parses gettext PO files.
 *
 * @package Titon\Io\Reader
 */
class PoReader extends AbstractReader {

    /**
     * {@inheritdoc}
     *
     * @throws \Titon\Io\Exception\ReadErrorException
     */
    public function read(): Map<string, mixed> {
        return $this->cache([__METHOD__, $this->path()], function() {
            if ($this->exists()) {
                return $this->_parse();
            }

            throw new ReadErrorException(sprintf('PoReader failed to parse %s', $this->name()));
        });
    }

    /**
     * Parse out the po contents.
     *
     * @return Map<string, mixed>
     */
    protected function _parse(): Map<string, mixed> {
        $lines = file($this->path(), FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
        $data = Map {};
        $key = '';
        $value = '';
        $plural = false;

        foreach ($lines as $line) {
            // Comment or empty line
            if ($line[0] === '#' || empty($line)) {
                continue;

            // Multiline value
            } else if ($line[0] === '"') {
                $value .= "\n" . self::dequote($line);

            // Key
            } else if (mb_strpos($line, 'msgid') === 0) {
                // Save the previous value
                if ($key !== '') {
                    $data[$key] = $value;
                    $value = '';
                }

                $key = self::dequote($line);

            // Message
            } else if (mb_strpos($line, 'msgstr') === 0) {
                // msgstr[n]
                if ($line[6] === '[') {
                    $val = self::dequote($line);

                    if ($plural) {
                        $value[] = $val;
                    } else {
                        $value = [$val];
                        $plural = true;
                    }

                // msgstr
                } else {
                    $value = self::dequote($line);
                    $plural = false;
                }
            }
        }

        // Grab the last value
        if ($key !== '') {
            $data[$key] = $value;
        }

        return $data;
    }

    /**
     * Remove the quotes from a message string.
     *
     * @param string $string
     * @return string
     */
    public static function dequote(string $string): string {
        return mb_substr(mb_substr($string, mb_strpos($string, '"')), 1, -1);
    }

}