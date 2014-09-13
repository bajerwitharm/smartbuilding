// send text email
sendEmail({
  subject: "Hello, World!",
  text: "This mail has been sent from the frontend",
  to: "joe@exam.pl"
})

// send multipart text / html email
sendEmail({
  subject: "Hello, World!",
  text: "This mail has been sent from the frontend",
  html: "<p>This mail has been sent from the frontend</p>",
  to: "joe@exam.pl"
})

// send multipart with attachment
sendEmail({
  subject: "Hello, World!",
  text: "This mail has been sent from the frontend",
  html: "<p>This mail has been sent from the frontend</p>",
  to: "gregor@martynus.net",
  attachments: [ 
    convert( document.body ).to("screenshot.png"),
    { filename: "info.text", data: "Some info about the page"}
  ]
})