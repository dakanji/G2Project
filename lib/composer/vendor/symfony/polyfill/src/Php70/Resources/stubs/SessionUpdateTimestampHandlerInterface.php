<?php
interface SessionUpdateTimestampHandlerInterface
{
    /**
     * Checks if a session identifier already exists or not.
     *
     * @param string $key
     *
     * @return bool
     */
    public function validateId($key);

    /**
     * Updates the timestamp of a session when its data did not change.
     *
     * @param string $key
     * @param string $val
     *
     * @return bool
     */
    public function updateTimestamp($key, $val);
}

